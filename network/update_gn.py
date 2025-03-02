import os

# ["module name", "path to files.gni", "directories with sources", "directories to ignore (optional)"]

modules = [
    ['common_gui', './src/common_gui/', ['./src/common_gui/']],
    ['common_base', './src/common/base/', ['./src/common/base/']],
    ['common_utils', './src/common/utils/', ['./src/common/utils/']],
    ['common_math', './src/common/math/', ['./src/common/math/']],
    ['reporters', './src/common/reporters/', ['./src/common/reporters/']],
    ['common_os', './src/common/os/', ['./src/common/os/']],
    ['common_parallel', './src/common/parallel/', ['./src/common/parallel/']],
    ['app_gui', './src/app_gui/', ['./src/app_gui']],
    ['workflow', './src/workflow/', ['./src/workflow']],
    ['objects_data', './src/nd/objects_data/', ['./src/nd/objects_data/']],
    ['unit_manager', './src/nd/kernel/unit_manager/', ['./src/nd/kernel/unit_manager/']],
    ['nd', './src/nd/', [ './src/nd/'], ['./src/nd/kernel/unit_manager/', './src/nd/objects_data/']],
    ['pvt', './src/pvt/', ['./src/pvt/']],
]

def write_list_to_file (file, module, list):
    fp = open (file, 'w')

    fp.write (module + '_sources = [\n')
    for each in list:
        fp.write (f'  "/{each[1:]}",\n')
    fp.write (']')
    fp.close ()

def cc_file_extention (file_name, exclutions):
    extension = file_name[file_name.find ('.'):]
    for exclude in exclutions:
        if extension == exclude:
            return False
    return True

def ignore_file (file_path, ignore_dirs):
    for ignore_dir in ignore_dirs:
        if file_path.find (ignore_dir) >= 0:
            return True
    return False

def get_all_files (source, ignore_dirs, ignore_files):
    files = []
    for root, subdirs, file_names in os.walk (source):
        for file_name in file_names:
            file_path = os.path.join (root, file_name)
            if cc_file_extention (file_name, ignore_files) and not ignore_file (file_path, ignore_dirs):
                files.append (file_path)
    return files

def check_modules ():
    for module in modules:
        files = []
        ignore_dirs = module[3] if len (module) > 3 else []
        for source in module[2]:
            files += get_all_files (source, ignore_dirs, ['.gn', '.gni'])
        write_list_to_file (module[1] + 'files.gni', module[0], sorted (files))

def main ():
    check_modules ()

main ()

