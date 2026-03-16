Import("env")
import shutil



def after_build(source, target, env):
    elf_path = str(target[0])
    shutil.copy(elf_path, "firmware.elf")

env.AddPostAction("buildprog", after_build)
