#!/usr/bin/python3

import os
import sys
import getopt

def usage():
    print('Usage: ' + os.path.basename(sys.argv[0]) + ' <PluginName>')

def get_file_list(dir, alowed_suffixes):
    files = []
    for f in os.listdir(dir):
        if not os.path.isdir(dir + '/' + f):
            fname, fsuffix = os.path.splitext(f)
            if fsuffix in alowed_suffixes:
                files.append(os.path.normpath(f))
    return files

def process_file(src, dst, new_name):
    template_name = 'Template'
    with open(dst, 'wt') as out_file:
        for line in open(src, 'rt'):
            new_line = line.replace(template_name, new_name)
            new_line = new_line.replace(template_name.lower(), new_name.lower())
            new_line = new_line.replace(template_name.upper(), new_name.upper())
            out_file.write(new_line)
    print(os.path.basename(src) + ' => ' + os.path.basename(dst))
    
def copy_from_template(new_name, alowed_suffixes=['*']):
    template_name = 'Template'
    template_dir = template_name.lower()
    new_dir = new_name.lower()
    base_dir = '../src/plugins/'
    try:
        os.mkdir(base_dir + new_dir)
    except:
        pass
    file_list = get_file_list(base_dir + template_dir, alowed_suffixes)
    for f in file_list:
        new_file = f.replace(template_name, new_name)
        new_file = new_file.replace(template_dir, new_dir)
        process_file(base_dir + template_dir + '/' + f,
                     base_dir + new_dir + '/' + new_file,
                     new_name)

# ---------- Main ----------

if __name__ == '__main__':
    try:
        opts, args = getopt.gnu_getopt(sys.argv[1:], 'h', ['help'])
    except:
        usage()
        sys.exit(2)

    if len(args) < 1:
        usage()
        sys.exit(2)

    for o, a in opts:
        if o in ('-h', '--help'):
            usage()
            sys.exit(0)
        
    plugin_name = args[0]
    copy_from_template(plugin_name, ['.cpp', '.h', '.in', '.qbs'])
