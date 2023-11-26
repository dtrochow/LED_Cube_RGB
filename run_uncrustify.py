from subprocess import Popen
import sys
import os
import argparse


REPO_PATH = os.path.dirname(os.path.realpath(__file__))
UNCRUSTIFY_CONFIG_PATH = os.path.join(REPO_PATH, "uncrustify.cfg")
EXTENSIONS_TO_PARSE = ("c", "cc", "h", "hh", "cpp", "hpp")
EXCLUDED_DIRECTORIES = (".git", "build")


class UncrustifyParser():
    def __init__(self, repo_path, config_path, extensions, exceluded_dirs=[]):
        self._repo_path = repo_path
        self._extensions = extensions
        self._excluded_dirs = exceluded_dirs
        self._uncrustify_cmd = ['uncrustify', '-c', config_path]
        self._options = []
        
    def add_option(self, option, value=None):
        if value:
            self._options.extend([option, value])
        else:
            self._options.append(option)

    def _search_for_files_to_parse(self):
        """
        return: (list) File paths to parse by uncrustify tool
        """
        all_files = []
        for root, dirs, files in os.walk(self._repo_path):
            dirs[:] = [d for d in dirs if d not in self._excluded_dirs]
            for f in files:
                all_files.append(os.path.relpath(os.path.join(root, f), '.'))
        return [os.path.join(REPO_PATH, d) for d in all_files if d.split('.')[-1] in self._extensions]
    
    def _create_cmd(self, file_path):
        cmd = self._uncrustify_cmd.copy()
        cmd.extend(self._options)
        cmd.append(file_path)
        return cmd
    
    def run(self):
        files_to_parse = self._search_for_files_to_parse()
        for file_path in files_to_parse:
            status = Popen(self._create_cmd(file_path), cwd=self._repo_path).wait()
            if (status != 0):
                raise SystemExit("Uncrustify command failure.");


def main(args):
    uncrustify = UncrustifyParser(REPO_PATH, UNCRUSTIFY_CONFIG_PATH, 
                                  EXTENSIONS_TO_PARSE, EXCLUDED_DIRECTORIES)
    if (args.check):
        uncrustify.add_option('--check')
    else:
        uncrustify.add_option('--no-backup')
    uncrustify.run()


def parse_args():
    parser = argparse.ArgumentParser(description="Simple script with a boolean parameter.")

    parser.add_argument('-c', '--check', action='store_true', help='Check if uncrustify passes on all files')
    return parser.parse_args()


if __name__ == "__main__":
    ret = main(parse_args())
    sys.exit(ret)
