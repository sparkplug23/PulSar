Import('env')
import os
import shutil

## Generate FirmwareCustom using Example

if os.path.isfile("HomeControlSystem/0_ConfigUser/mFirmwareCustom_Secret.h"):
    print ("********* using mFirmwareCustom_Secret.h as planned ***********")
else: 
    print ("********* WARNING **************************** Creating mFirmwareCustom_Secret.h from mFirmwareCustom_Example ***********")
    # Read from example
    f = open("HomeControlSystem/0_ConfigUser/ExamplesUsedToGenerateSecret/mFirmwareCustom_Example.h", "r")
    read_input = f.read()

    # Change header guards
    write_output = read_input.replace("MFIRMWARECUSTOM_EXAMPLE_HEADER", "MFIRMWARECUSTOM_SECRET_HEADER")

    # Open (and create) a new file to write in to
    f = open("HomeControlSystem/0_ConfigUser/mFirmwareCustom_Secret.h", "a")
    f.write(write_output)


## Generate mUserConfig using Example

if os.path.isfile("HomeControlSystem/0_ConfigUser/mUserConfig_Secret.h"):
    print ("********* using mUserConfig_Secret.h as planned ***********")
else: 
    print ("********* WARNING **************************** Creating mUserConfig_Secret.h from mUserConfig_Example ***********")
    # Read from example
    f = open("HomeControlSystem/0_ConfigUser/ExamplesUsedToGenerateSecret/mUserConfig_Example.h", "r")
    read_input = f.read()
    
    # Change header guards
    write_output = read_input.replace("MUSERCONFIG_EXAMPLE_HEADER", "MUSERCONFIG_SECRET_HEADER")

    # Open (and create) a new file to write in to
    f = open("HomeControlSystem/0_ConfigUser/mUserConfig_Secret.h", "a")
    f.write(write_output)


