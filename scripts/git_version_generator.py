# -*- coding: utf-8 -*-
"""
@brief
@author: Graham Riches
@date: Wed May 13 15:44:03 2020
@description
    module to generate automatic git version string header files
"""

import subprocess
import re
import os
import sys


GIT_REGEX_STR = '(\d+)\.(\d+)\.(\d+)-(\d+)-([\w\d]+)-?(dirty)?'


REGEX_TAGS = ['version_major', 'version_minor', 'version_micro',
              'commits_past_head', 'version_hash', 'is_dirty']


class GitVersionGenerator:
    def __init__(self):
        # initialize the default version
        self._version = {'version_major': 0, 'version_minor': 0,
                         'version_micro': 0, 'version_hash': '0000000',
                         'commits_past_head': 0, 'is_dirty': ''}

        self._types = {'version_major': int, 'version_minor': int,
                       'version_micro': int, 'version_hash': str,
                       'commits_past_head': int, 'is_dirty': str}

        self._header_consts = {'version_major': 'GIT_VERSION_MAJOR',
                               'version_minor': 'GIT_VERSION_MINOR',
                               'version_micro': 'GIT_VERSION_MICRO',
                               'version_hash': 'GIT_SHA',
                               'commits_past_head': 'GIT_COMMIT_PAST_HEAD',
                               'is_dirty': 'GIT_DIRTY'}

    def run_git_cmd(self, command: str) -> str:
        """ run a git command and return the output as a string """
        output = subprocess.check_output('git {}'.format(command), shell=True)
        return str(output)

    def get_git_description(self):
        """ get the git description """
        options = ['--dirty', '--tags', '--long']
        command = 'describe {}'.format(' '.join(opt for opt in options))
        git_str = self.run_git_cmd(command)
        tags = git_str.split('/')
        version_tags = tags[-1]
        version_info = re.findall(GIT_REGEX_STR,
                                  version_tags)
        if len(version_info) == 0:
            return False
        for idx, version_item in enumerate(version_info[0]):
            self._version[REGEX_TAGS[idx]] = self._types[REGEX_TAGS[idx]](version_item)

        # extra check to make sure that commits past the head get tagged as dirty
        if self._version['commits_past_head'] > 0:
            self._version['is_dirty'] = 'dirty'
        if self._version['is_dirty'] == 'dirty':
            self._version['version_hash'] += '+dirty'
        return True

    def print_version(self):
        """ print out the git version """
        for key in self._version:
            print('{}: {}'.format(key, self._version[key]))

    def generate_version_header(self, filename):
        """ generate a C version header file """
        with open(filename, 'w') as header_file:
            # strip out any path information
            file = os.path.split(filename)[-1]
            header_name = file.strip('.h')
            header_guard = '__{}_H'.format(header_name.upper())
            header_file.writelines('#ifndef {}\n'.format(header_guard))
            header_file.writelines('#define {}\n'.format(header_guard))
            for key in self._version:
                if self._types[key] == str:
                    header_file.writelines('#define {} "{}"\n'.format(self._header_consts[key],
                                                                      self._version[key]))
                else:
                    header_file.writelines('#define {} {}\n'.format(self._header_consts[key],
                                                                    self._version[key]))
            header_file.writelines('#endif /* {} */\n'.format(header_guard))


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('invalid number of arguments. Usage: python git_version_generator.py [FILENAME]')
        exit()
    version_gen = GitVersionGenerator()
    if not version_gen.get_git_description():
        print('failed to get git description')
        exit()
    version_gen.print_version()
    version_gen.generate_version_header(sys.argv[1])
