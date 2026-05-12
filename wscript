import subprocess


APPNAME = "minishell"
VERSION = "1.0"
top = "."
out = "wbuild"


def configure(conf):
	conf.env.CC		= subprocess.run("readlink $(which cc)", shell=True, capture_output=True, text=True).stdout[:-1]
	conf.env.CFLAGS = [
		"-Wall",
		"-Wextra",
		"-Werror",
		"-Wno-unused-variable",
		"-fshort-enums",
		"-O2",
		"-march=native"
	]
	conf.env.LIB = [
		"readline"
	]
	conf.load(conf.env.CC.split("/")[-1])


def build(bld):
	bld.env.append_value("INCLUDES_ROOT", [bld.path.find_dir('includes')])
	bld.recurse("src")
	bld.recurse("libft")
