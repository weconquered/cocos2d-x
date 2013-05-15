#!/usr/bin/python

import sys
import subprocess
import os
import inspect

class Generator(object):
    """
    JSB Bytecode Generator Class
    """

    def __init__(self, src_dir, dst_dir, use_closure_compiler):
        """

        Arguments:
        - `src_dir`:
        - `dst_dir`:
        """
        self._src_dir = src_dir
        self._dst_dir = dst_dir == None and src_dir or dst_dir
        self._use_closure_compiler = use_closure_compiler != 0
        self._success = []
        self._failure = []
        self._jsfiles = []
        self._compressed_js_path = os.path.join(self._dst_dir, "__cocos2dx_jsb_game_tmpfile__.js")
        self._compressed_jsc_path = os.path.join(self._dst_dir,"game.jsc")

        print "src dir:"+self._src_dir +",dst dir:"+self._dst_dir+",use closure compiler:"+str(self._use_closure_compiler)

    def get_relative_path(self, jsfile):
        try:
            pos = jsfile.index(self._src_dir)
            if pos != 0:
                raise Exception("cannot find src directory in file path.")
            return jsfile[len(self._src_dir)+1:]
        except ValueError:
            raise Exception("cannot find src directory in file path.")

    def get_output_file_path(self, jsfile):
        """
        """
        # create folder for generated file
        jsc_filepath = os.path.join(self._dst_dir, self.get_relative_path(jsfile)+"c")
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
        print "test....."
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
        jsfiles = " --js ".join(self._jsfiles)
        compiler_jar_path = os.path.join("closure-compiler", "compiler.jar")
        print jsfiles

        command = "java -jar %s --js %s --js_output_file %s" % (compiler_jar_path, jsfiles, self._compressed_js_path)
        print command
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
                    self._jsfiles.append(path)

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
        self.deep_iterate_dir(self._src_dir)

        if self._use_closure_compiler == True:
            self.compress_js()
            self.compile_js(self._compressed_js_path, self._compressed_jsc_path)
            # remove tmp compressed file
            os.remove(self._compressed_js_path)
        else:
            for jsfile in self._jsfiles:
                self.compile_js(jsfile, self.get_output_file_path(jsfile))

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
                      action="store", type="string", dest="src_dir",
                      help="source directory")

    parser.add_option("-d", "--dst",
                      action="store", type="string", dest="dst_dir",
                      help="destination directory")

    parser.add_option("-c", "--closure",
                      action="store", type="int", dest="use_closure_compiler", default=0,
                      help="use closure compiler to package all js files to a big file")

    (options, args) = parser.parse_args()

    if options.src_dir == None:
        raise Exception("Please set src folder by \"-s\" or \"-src\", run ./jscompile.py -h for the usage ")
    elif os.path.exists(options.src_dir) == False:
        raise Exception("Error: dir (%s) doesn't exist..." % (options.src_dir))

    print options

    generator = Generator(options.src_dir, options.dst_dir, options.use_closure_compiler)
    generator.run()

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print e
        sys.exit(1)
