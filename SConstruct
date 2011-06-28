import os
import sys
import platform

def CheckMacports(context):
	context.Message('Looking for macports... ')
	prog = context.env.WhereIs('port')
	if prog:
		ret = 1
		prefix = prog.rsplit(os.sep, 2)[0]
		context.env.Append(LIBPATH = [prefix + os.sep + 'lib'], CPPPATH = [prefix + os.sep + 'include'])
		context.Message( 'Adding port lib and include path')
	else:
		ret = 0
        
	context.Result(ret)
	return int(ret)


env = Environment(ENV = { 'PATH' : os.environ['PATH'], 'HOME' : os.environ['HOME'] })

conf = Configure(env, custom_tests = { 'CheckMacports' : CheckMacports })

if sys.platform == 'darwin':
	conf.CheckMacports()

env = conf.Finish()

env.AppendUnique(LIBS = ['boost_filesystem-mt', 'boost_iostreams-mt', 'boost_system-mt', 'boost_thread-mt'])

env.Append(CXXFLAGS = ['-O3'])
env.Append(CXXFLAGS = ['-W'])
env.Append(CXXFLAGS = ['-Wall'])
env.Program(target = "mapreduce", source = Glob('*.cpp'))

