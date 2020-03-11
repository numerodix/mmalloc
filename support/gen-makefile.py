#!/usr/bin/env python

import os


COMPILER = 'cc'

CFLAGS = [
    '-std=gnu11',
    '-pedantic',
    '-Wall',
    '-Werror',
    '-Wextra',
    '-g',
    '-pthread',
]

SOFLAGS_PUBLIC = [
    '-shared',
    '-fPIC',
    '-fvisibility=hidden',
    '-DEXPORT_REAL_API',
    '-DTRACE',
]

SOFLAGS_PRIVATE = [
    '-shared',
    '-fPIC',
]

BINARIES_LOCATION = 'bin'
HEADERS_LOCATION = 'include'
SOURCES_LOCATION = 'src'
TESTS_LOCATION = 'tests'

PUBLIC_SHARED_LIB_NAME = os.path.join(BINARIES_LOCATION, 'libmalloc.so')
PRIVATE_SHARED_LIB_NAME = os.path.join(BINARIES_LOCATION, 'libmmalloc.so')


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

    @property
    def linking_name(self):
        filename = os.path.basename(self.name)
        name, _ = os.path.splitext(filename)
        return name.replace('lib', '')


class Generator:
    def __init__(self, outfile):
        self.outfile = outfile

    # filesystem

    def get_cfiles(self, location):
        names = os.listdir(location)
        names.sort()
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
        soflags_public = ' '.join(SOFLAGS_PUBLIC)
        soflags_private = ' '.join(SOFLAGS_PRIVATE)
        lines = [
            'CC := %s' % COMPILER,
            'CFLAGS := %s' % cflags,
            'SOFLAGS_PUBLIC := %s' % soflags_public,
            'SOFLAGS_PRIVATE := %s' % soflags_private,
        ]
        return self.to_block(lines)

    def generate_test_target(self, testfile, sources, headers, shared_libs):
        target = testfile.as_binary

        dep_files = [testfile.filepath] + [lib.name for lib in shared_libs]
        deps = ' '.join(dep_files)
        libs = ' '.join(['-l%s' % lib.linking_name for lib in shared_libs])

        block = (
            '%(target)s: %(deps)s\n'
            '\t@mkdir -p %(binaries_loc)s\n'
            '\t$(CC) $(CFLAGS) '
            '-I %(header_loc)s '
            '-Wl,-rpath=`pwd`/%(binaries_loc)s '
            '-L %(binaries_loc)s '
            '-o $@ %(source)s '
            '%(libs)s'
        ) % dict(
            target=target,
            deps=deps,
            binaries_loc=BINARIES_LOCATION,
            header_loc=HEADERS_LOCATION,
            source=testfile.filepath,
            libs=libs,
        )

        return Target(name=target, block=block)

    def generate_test_targets(self, shared_libs):
        headers = self.get_header_files()
        sources = self.get_source_files()
        tests = self.get_test_files()

        targets = [
            self.generate_test_target(testfile, sources, headers, shared_libs)
            for testfile in tests
        ]
        targets.sort(key=lambda target: target.name)

        return targets

    def generate_shared_lib_targets(self):
        headers = self.get_header_files()
        sources = self.get_source_files()

        dep_files = sources + headers
        deps = ' '.join([dep.filepath for dep in dep_files])
        inputs = ' '.join([source.filepath for source in sources])

        public_name = PUBLIC_SHARED_LIB_NAME
        block = (
            '%(target)s: %(deps)s\n'
            '\t@mkdir -p %(binaries_loc)s\n'
            '\t$(CC) $(CFLAGS) $(SOFLAGS_PUBLIC) -I %(header_loc)s -o $@ %(inputs)s'
        ) % dict(
            target=public_name,
            deps=deps,
            binaries_loc=BINARIES_LOCATION,
            header_loc=HEADERS_LOCATION,
            inputs=inputs,
        )
        public_target = Target(name=public_name, block=block)

        private_name = PRIVATE_SHARED_LIB_NAME
        block = (
            '%(target)s: %(deps)s\n'
            '\t@mkdir -p %(binaries_loc)s\n'
            '\t$(CC) $(CFLAGS) $(SOFLAGS_PRIVATE) -I %(header_loc)s -o $@ %(inputs)s'
        ) % dict(
            target=private_name,
            deps=deps,
            binaries_loc=BINARIES_LOCATION,
            header_loc=HEADERS_LOCATION,
            inputs=inputs,
        )
        private_target = Target(name=private_name, block=block)

        return [public_target, private_target]

    def generate_group_targets(self, test_targets, shared_lib_targets):
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
        tests_block = '%s: %s %s' % (
            tests_name, build_tests_name, run_tests_name)
        tests = Target(name=tests_name, block=tests_block)

        build_shared_libs_name = 'build-shared-libraries'
        deps = ' '.join([target.name for target in shared_lib_targets])
        build_shared_lib_block = '%s: %s' % (build_shared_libs_name, deps)
        build_shared_libs = Target(
            name=build_shared_libs_name,
            block=build_shared_lib_block,
        )

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
            build_shared_libs,
            clean,
        ]

        return targets

    def generate_phony(self, targets):
        deps = ' '.join([target.name for target in targets])
        block = '.PHONY: %(deps)s' % dict(deps=deps)
        return block

    def generate_file(self):
        constants_block = self.generate_constants()

        shared_lib_targets = self.generate_shared_lib_targets()
        [public_shared_lib_target, private_shared_lib_target] = shared_lib_targets
        shared_lib_targets_block = self.to_block(
            [target.block for target in shared_lib_targets],
            sep=2
        )

        test_targets = self.generate_test_targets([private_shared_lib_target])
        test_targets_block = self.to_block(
            [target.block for target in test_targets],
            sep=2
        )

        group_targets = self.generate_group_targets(
            test_targets,
            [public_shared_lib_target],
        )
        group_targets_block = self.to_block(
            [target.block for target in group_targets],
            sep=2
        )

        phony_block = self.generate_phony(group_targets)

        blocks = [
            phony_block,
            constants_block,
            shared_lib_targets_block,
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
