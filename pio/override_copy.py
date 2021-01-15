Import('env')
import os
import shutil

# Check if secret file exists, if not, create it using the template example

if os.path.isfile("HomeControlSystem/0_ConfigUser/mUserConfigSecret.h"):
    print ("********* using mUserConfigSecret.h as planned ***********")
else: 
    print ("********* WARNING **************************** Creating mUserConfigSecret.h from mUserConfigSecret_Example ***********")
    f = open("tools/mUserConfigSecret_Example.h", "r")
    # print(f.read())

    a = f.read()
    # print(a.replace("mUserConfigSecret_Example_H", "mUserConfigSecret_H"))

    
    f = open("HomeControlSystem/0_ConfigUser/mUserConfigSecret.h", "a")
    f.write(a.replace("mUserConfigSecret_Example_H", "mUserConfigSecret_H"))

    # shutil.copy("tools/mUserConfigSecret_Example.h", "HomeControlSystem/0_ConfigUser/mUserConfigSecret.h")
