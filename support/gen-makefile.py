#!/usr/bin/env python

import os


COMPILER = 'cc'

CFLAGS = [
    '-std=gnu11',
    '-Wall',
    '-Werror',
    '-g',
]

BINARIES_LOCATION = 'bin'
HEADERS_LOCATION = 'include'
SOURCES_LOCATION = 'src'
TESTS_LOCATION = 'tests'


class CFile:
    def __init__(self, path, name):
        self.path = path
        self.name = name

    @property
    def as_binary(self):
        name, _ = os.path.splitext(self.name)
        return os.path.join(BINARIES_LOCATION, name)

    @property
    def filepath(self):
        return os.path.join(self.path, self.name)


class Target:
    def __init__(self, name, block):
        self.name = name
        self.block = block


class Generator:
    def __init__(self, outfile):
        self.outfile = outfile

    # filesystem

    def get_cfiles(self, location):
        names = os.listdir(location)
        cfiles = [CFile(location, name) for name in names]
        return cfiles

    def get_header_files(self):
        return self.get_cfiles(HEADERS_LOCATION)

    def get_source_files(self):
        return self.get_cfiles(SOURCES_LOCATION)

    def get_test_files(self):
        return self.get_cfiles(TESTS_LOCATION)

    # formatting

    def to_block(self, lines, sep=1):
        delim = '\n' * sep
        return '%s\n' % delim.join(lines)

    # codegen

    def generate_constants(self):
        cflags = ' '.join(CFLAGS)
        lines = [
            'CFLAGS := %s' % cflags,
        ]
        return self.to_block(lines)

    def generate_test_target(self, testfile, sources, headers):
        target = testfile.as_binary

        dep_files = [testfile] + sources + headers
        deps = ' '.join([dep.filepath for dep in dep_files])
        inputs = ' '.join([source.filepath for source in sources])

        block = (
            '%(target)s: %(deps)s\n'
            '\t@mkdir -p %(binaries_loc)s\n'
            '\t%(compiler)s $(CFLAGS) -I %(header_loc)s -o $@ %(source)s %(inputs)s'
        ) % dict(
            target=target,
            deps=deps,
            binaries_loc=BINARIES_LOCATION,
            compiler=COMPILER,
            header_loc=HEADERS_LOCATION,
            source=testfile.filepath,
            inputs=inputs,
        )

        return Target(name=target, block=block)

    def generate_test_targets(self):
        headers = self.get_header_files()
        sources = self.get_source_files()
        tests = self.get_test_files()

        targets = [
            self.generate_test_target(testfile, sources, headers)
            for testfile in tests
        ]
        targets.sort(key=lambda target: target.name)

        return targets

    def generate_group_targets(self, test_targets):
        build_tests_name = 'build-tests'
        deps = ' '.join([target.name for target in test_targets])
        build_tests_block = '%s: %s' % (build_tests_name, deps)
        build_tests = Target(name=build_tests_name, block=build_tests_block)

        run_tests_name = 'run-tests'
        stmts = self.to_block(
            '\t./%s' % target.name for target in test_targets
        )
        run_tests_block = (
            '%(target)s:\n'
            '%(stmts)s'
        ) % dict(
            target=run_tests_name,
            stmts=stmts,
        )
        run_tests = Target(name=run_tests_name, block=run_tests_block)

        tests_name = 'tests'
        tests_block = '%s: %s %s' % (tests_name, build_tests_name, run_tests_name)
        tests = Target(name=tests_name, block=tests_block)

        clean_name = 'clean'
        clean_block = (
            '%(target)s:\n'
            '\trm -rf %(binaries_loc)s'
        ) % dict(
            target=clean_name,
            binaries_loc=BINARIES_LOCATION,
        )
        clean = Target(name=clean_name, block=clean_block)

        targets = [
            build_tests,
            run_tests,
            tests,
            clean,
        ]

        return targets

    def generate_phony(self, targets):
        deps = ' '.join([target.name for target in targets])
        block = '.PHONY: %(deps)s' % dict(deps=deps)
        return block

    def generate_file(self):
        constants_block = self.generate_constants()

        test_targets = self.generate_test_targets()
        test_targets_block = self.to_block(
            [target.block for target in test_targets],
            sep=2
        )

        group_targets = self.generate_group_targets(test_targets)
        group_targets_block = self.to_block(
            [target.block for target in group_targets],
            sep=2
        )

        phony_block = self.generate_phony(group_targets)

        blocks = [
            phony_block,
            constants_block,
            test_targets_block,
            group_targets_block,
        ]

        contents = self.to_block(blocks, sep=2)
        return contents

    def write_file(self):
        contents = self.generate_file()

        with open(self.outfile, 'wb+') as fl:
            fl.write(contents)


if __name__ == '__main__':
    generator = Generator(
        outfile='Makefile'
    )
    generator.write_file()
