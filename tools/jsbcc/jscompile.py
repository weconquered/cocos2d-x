#!/usr/bin/python

import sys
import subprocess
import os
import json
import copy

class Generator(object):
    """
    JSB Bytecode Generator Class
    """

    def __init__(self, src_dir_arr, dst_dir, use_closure_compiler, config):
        """

        Arguments:
        - `src_dir_arr`:
        - `dst_dir_arr`:
        """
        self._current_src_dir = None
        self._src_dir_arr = src_dir_arr
        self._dst_dir = dst_dir
        self._use_closure_compiler = use_closure_compiler != 0
        self._config = config

        self._success = []
        self._failure = []
        self._js_files = {}
        self._compressed_js_path = os.path.join(self._dst_dir, "__cocos2dx_jsb_game_tmpfile__.js")
        self._compressed_jsc_path = os.path.join(self._dst_dir,"game.jsc")

    def get_relative_path(self, jsfile):
        try:
            print "current src dir: "+self._current_src_dir
            pos = jsfile.index(self._current_src_dir)
            if pos != 0:
                raise Exception("cannot find src directory in file path.")
            print "origin js path: "+ jsfile
            print "relative path: "+jsfile[len(self._current_src_dir)+1:]
            return jsfile[len(self._current_src_dir)+1:]
        except ValueError:
            raise Exception("cannot find src directory in file path.")

    def get_output_file_path(self, jsfile):
        """
        """
        # create folder for generated file
        jsc_filepath = ""
        relative_path = self.get_relative_path(jsfile)+"c"
        if len(self._js_files) > 1:
            jsc_filepath = os.path.join(self._dst_dir, self._current_src_dir, relative_path)
        else:
            jsc_filepath = os.path.join(self._dst_dir, relative_path)

        dst_rootpath = os.path.split(jsc_filepath)[0]
        try:
            # print "creating dir (%s)" % (dst_rootpath)
            os.makedirs(dst_rootpath)
        except OSError:
            if os.path.exists(dst_rootpath) == False:
                # There was an error on creation, so make sure we know about it
                raise Exception("Error: cannot create folder in "+dst_rootpath)

        # print "return jsc path: "+jsc_filepath
        return jsc_filepath

    def compile_js(self, jsfile, output_file):
        """
        Compile js file
        """
        print "compiling js (%s) to bytecode..." % (jsfile)

        ret = subprocess.call(["jsbcc", jsfile, output_file])
        if ret == 0:
            self._success.append(jsfile)
        else:
            self._failure.append(jsfile)
        print "----------------------------------------"

    def compress_js(self):
        """
        """
        jsfiles = ""
        for src_dir in self._src_dir_arr:
            print "\n----------src:"+src_dir
            jsfiles = jsfiles + " --js ".join(self._js_files[src_dir]) + " "

        compiler_jar_path = os.path.join("closure-compiler", "compiler.jar")
        command = "java -jar %s --js %s --js_output_file %s" % (compiler_jar_path, jsfiles, self._compressed_js_path)
        print "command:"+command
        ret = subprocess.call(command, shell=True)
        if ret == 0:
            print "js files were compressed successfully..."
        else:
            print "js files were compressed unsuccessfully..."

    def deep_iterate_dir(self, rootDir):
        for lists in os.listdir(rootDir):
            path = os.path.join(rootDir, lists)
            if os.path.isdir(path):
                self.deep_iterate_dir(path)
            elif os.path.isfile(path):
                if os.path.splitext(path)[1] == ".js":
                    self._js_files[self._current_src_dir].append(path)


    def js_filename_pre_order_compare(self, a, b):
        """
        """
        pre_order = self._config["pre_order"]

        if a in pre_order and not b in pre_order:
            return -1
        elif not a in pre_order and b in pre_order:
            return 1
        elif a in pre_order and b in pre_order:
            if pre_order.index(a) > pre_order.index(b):
                return 1
            elif pre_order.index(a) < pre_order.index(b):
                return -1
            else:
                return 0
        else:
            return 0


    def js_filename_post_order_compare(self, a, b):
        """
        """
        post_order = self._config["post_order"]

        if a in post_order and not b in post_order:
            return 1
        elif not a in post_order and b in post_order:
            return -1
        elif a in post_order and b in post_order:
            if post_order.index(a) > post_order.index(b):
                return 1
            elif post_order.index(a) < post_order.index(b):
                return -1
            else:
                return 0
        else:
            return 0

    def reorder_js_files(self):
        if self._config == None:
            return

        # print "before:"+str(self._js_files)

        for src_dir in self._js_files:
            # Remove file in exclude list
            need_remove_arr = []
            for jsfile in self._js_files[src_dir]:
                for exclude_file in self._config["exclude"]:
                    if jsfile.find(exclude_file) != -1:
                        print "remove:" + jsfile
                        need_remove_arr.append(jsfile)

            for need_remove in need_remove_arr:
                self._js_files[src_dir].remove(need_remove)

            self._js_files[src_dir].sort(cmp=self.js_filename_pre_order_compare)
            self._js_files[src_dir].sort(cmp=self.js_filename_post_order_compare)

        # print '-------------------'
        # print "after:" + str(self._js_files)

    def handle_all_js_files(self):
        """
        Arguments:
        - `self`:
        """
        if self._use_closure_compiler == True:
            self.compress_js()
            self.compile_js(self._compressed_js_path, self._compressed_jsc_path)
            # remove tmp compressed file
            os.remove(self._compressed_js_path)
        else:
            for src_dir in self._src_dir_arr:
                for jsfile in self._js_files[src_dir]:
                    self._current_src_dir = src_dir
                    self.compile_js(jsfile, self.get_output_file_path(jsfile))

    def run(self):
        """
        """
        # create output directory
        try:
            os.makedirs(self._dst_dir)
        except OSError:
            if os.path.exists(self._dst_dir) == False:
                raise Exception("Error: cannot create folder in "+self._dst_dir)

        # deep iterate the src directory
        for src_dir in self._src_dir_arr:
            print "src dir:"+src_dir
            self._current_src_dir = src_dir
            self._js_files[self._current_src_dir] = []
            self.deep_iterate_dir(src_dir)

        self.reorder_js_files()
        self.handle_all_js_files()
        print "\nCompilation finished, (%d) files succeed, (%d) files fail." % (len(self._success), len(self._failure))
        if len(self._failure) > 0:
            print "Failure files are:"
            print self._failure
        print "------------------------------"

def main():
    """
    """
    from optparse import OptionParser

    parser = OptionParser("usage: %prog -s src_dir [-d dst_dir -c use_closure_compiler]")
    parser.add_option("-s", "--src",
                      action="append", type="string", dest="src_dir_arr",
                      help="source directory")

    parser.add_option("-d", "--dst",
                      action="store", type="string", dest="dst_dir",
                      help="destination directory")

    parser.add_option("-c", "--closure",
                      action="store", type="int", dest="use_closure_compiler", default=0,
                      help="use closure compiler to package all js files to a big file")

    (options, args) = parser.parse_args()

    if options.src_dir_arr == None:
        raise Exception("Please set source folder by \"-s\" or \"-src\", run ./jscompile.py -h for the usage ")
    elif options.dst_dir == None:
        raise Exception("Please set destination folder by \"-d\" or \"-dst\", run ./jscompile.py -h for the usage ")
    else:
        for src_dir in options.src_dir_arr:
            if os.path.exists(src_dir) == False:
                raise Exception("Error: dir (%s) doesn't exist..." % (src_dir))


    f = open("compiler-config.json")
    config = json.load(f)
    print config
    f.close()

    generator = Generator(options.src_dir_arr, options.dst_dir, options.use_closure_compiler, config)
    generator.run()

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print e
        sys.exit(1)
