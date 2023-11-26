import sys
import os
import argparse


SCRIPT_DIR_PATH = os.path.dirname(os.path.realpath(__file__))
CUBE_REPO_PATH = os.path.join(SCRIPT_DIR_PATH, os.pardir, os.pardir)
TEMPLATES_DIR_PATH = os.path.join(SCRIPT_DIR_PATH, "templates")
CMAKE_TEMPLATE_PATH = os.path.join(TEMPLATES_DIR_PATH, "CMakeLists")
HEADER_TEMPLATE_PATH = os.path.join(TEMPLATES_DIR_PATH, "header")
SOURCE_TEMPLATE_PATH = os.path.join(TEMPLATES_DIR_PATH, "source")
ANIMATIONS_CMAKELISTS_FILE_PATH = os.path.join(CUBE_REPO_PATH, "animations", "CMakeLists.txt")
ANIMATIONS_HEADER_PATH = os.path.join(CUBE_REPO_PATH, "animations", "include", "animations.hpp")
ANIMATION_RUNNER_MODULE_PATH = os.path.join(CUBE_REPO_PATH, "led_cube", "animation_runner")
ANIMATION_RUNNER_SOURCE_PATH = os.path.join(ANIMATION_RUNNER_MODULE_PATH, "animation_runner.cpp") 
ANIMATION_RUNNER_TYPES_PATH = os.path.join(ANIMATION_RUNNER_MODULE_PATH, "include", "animation_runner_types.hpp")

MODULE_NAME_PATTERN = "<<ANIMATION_NAME>>"
CLASS_NAME_PATTERN = "<<CLASS_NAME>>"
FILE_NAME_PATTERN = "<<FILE_NAME>>"


def create_animation_directory(args):
    animation_directory = os.path.join(CUBE_REPO_PATH, "animations", args.name.lower())
    if (os.path.isdir(animation_directory)):
        raise SystemExit(f"The {animation_directory} animation directory already exists!")
    os.makedirs(animation_directory)
    return animation_directory


def create_cmakelist_file(args, anim_directory, source_file_name):
    module_name = (args.name+"_animation").lower()
    cmakelists_file_path = os.path.join(anim_directory, "CMakeLists.txt")
    with open(cmakelists_file_path, 'w') as cmakelists:
        with open(CMAKE_TEMPLATE_PATH, 'r') as cmakelists_template:
            cmakelist_data = cmakelists_template.read().replace(FILE_NAME_PATTERN, source_file_name).replace(MODULE_NAME_PATTERN, module_name)
            cmakelists.write(cmakelist_data)
    return module_name


def create_header_file(anim_directory):
    class_name = ''.join([part[0].upper()+part[1:].lower() for part in args.name.split('_')])
    header_filename = args.name.lower()+".hpp"
    include_dir_path = os.path.join(anim_directory, "include")
    os.makedirs(include_dir_path)
    header_file_path = os.path.join(include_dir_path, header_filename)
    with open(header_file_path, 'w') as header:
        with open(HEADER_TEMPLATE_PATH, 'r') as header_template:
            header_data = header_template.read().replace(CLASS_NAME_PATTERN, class_name)
            header.write(header_data)
    return class_name, header_filename


def create_source_file(anim_directory, class_name, header_filename):
    source_filename = args.name.lower()+".cpp"
    source_file_path = os.path.join(anim_directory, source_filename)
    with open(source_file_path, 'w') as source:
        with open(SOURCE_TEMPLATE_PATH, 'r') as source_template:
            source_data = source_template.read().replace(CLASS_NAME_PATTERN, class_name).replace(FILE_NAME_PATTERN, header_filename)
            source.write(source_data)
    return source_filename


def add_cmake_dependencies(module_name, animation_directory):
    animation_directory_name = animation_directory.split(os.sep)[-1]
    with open(ANIMATIONS_CMAKELISTS_FILE_PATH, 'r') as cmakelists_file:
        cmake_data = cmakelists_file.readlines()
        set_animations_pos = cmake_data.index("set(animations_modules\n")
        cmake_data.insert(set_animations_pos+1, '\t'+module_name+'\n')
        cmake_data.insert(0, f"add_subdirectory({animation_directory_name})\n")
    with open(ANIMATIONS_CMAKELISTS_FILE_PATH, 'w') as cmakelists_file:
        cmakelists_file.writelines(cmake_data)


def add_animation_to_runner(args, class_name):
    animation_enum_value = args.name.upper()
    animations_table_entry = "{ "+f"AnimationType::{animation_enum_value}, new {class_name}() "+"},"
    with open(ANIMATION_RUNNER_TYPES_PATH, 'r') as types:
        types_data = types.readlines()
        animation_types_pos = types_data.index("enum class AnimationType : int {\n")
        types_data.insert(animation_types_pos+1, '\t'+animation_enum_value+',\n')
    with open(ANIMATION_RUNNER_TYPES_PATH, 'w') as types:
        types.writelines(types_data)
    with open(ANIMATION_RUNNER_SOURCE_PATH, 'r') as source:
        source_data = source.readlines()
        animations_objs_table_pos = source_data.index("    animations = {\n")
        source_data.insert(animations_objs_table_pos+1, '\t\t'+animations_table_entry+'\n')
    with open(ANIMATION_RUNNER_SOURCE_PATH, 'w') as source:
        source.writelines(source_data)


def add_animation_header(header_filename):
    with open(ANIMATIONS_HEADER_PATH, 'r') as animations_header:
        animations_header_data = animations_header.readlines()
        animations_header_data.insert(len(animations_header_data)-1, f'#include "{header_filename}"\n')
    with open(ANIMATIONS_HEADER_PATH, 'w') as animations_header:
        animations_header.writelines(animations_header_data)


def main(args):
    animation_directory = create_animation_directory(args)
    class_name, header_filename = create_header_file(animation_directory)
    source_filename = create_source_file(animation_directory, class_name, header_filename)
    module_name = create_cmakelist_file(args, animation_directory, source_filename)
    add_cmake_dependencies(module_name, animation_directory)
    add_animation_to_runner(args, class_name)
    add_animation_header(header_filename)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Script creating the animation module initialization.')
    parser.add_argument('--name', '-n',type=str,
                        help='Name of an animation')

    args = parser.parse_args()
    ret = main(args)
    sys.exit(ret)
