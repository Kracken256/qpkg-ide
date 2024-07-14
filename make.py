#!/usr/bin/env python3

# Compile the QUIX IDE Project for the user-specified platform

from sys import argv
from os import system, path, uname, getcwd, makedirs
from datetime import datetime
from subprocess import Popen, PIPE

# Define the platforms that are supported
platform_targets = {"ubuntu-22.04": "Ubuntu-22.04.Dockerfile"}

# Define the build types that are supported
#
# --debug: Moderate optimization, debug symbols, no compression.
# --test: Full optimization (includes LTO), no debug symbols, no compression.
# --trace: Full optimization (includes LTO), no debug symbols, no compression,
#          profiling enabled, code coverage enabled.
# --prerelease: Full optimization (includes LTO), debug symbols, compression, asserts disabled.
# --release: Full optimization (includes LTO), no debug symbols, compression, code signing (if applicable),
#            app bundle creation, ready to ship.
build_types = {"--debug": "Debug Build", "--test": "Test Build",
               "--trace": "Code Coverage and Profiling Build",
               "--prerelease": "Pre-Release Build",
               "--release": "Release Build (Production)"}
build_type_dirs = {"--debug": "debug", "--test": "test",
                   "--trace": "trace",
                   "--prerelease": "prerelease",
                   "--release": "release"}


# Check if the user has provided the correct number of arguments
if len(argv) < 3:
    print(
        "Usage: make.py <platform> [--debug, --test, --trace, --prerelease, --release]")
    exit(1)

# Check if the user has provided a valid platform name
if argv[1] not in platform_targets:
    print(f"Requested platform `{argv[1]}` is not supported.")
    print("+" + "-" * 40 + "+\n")
    print("Supported platforms: ")
    for platform in platform_targets:
        print(' -', platform)
    exit(1)

# Check if the user has provided a valid build type
if argv[2] not in build_types:
    print(f"Requested build type `{argv[2]}` is not supported.")
    print("+" + "-" * 40 + "+\n")
    print("Supported build types: ")
    for build_type in build_types:
        print('  ' + build_type, "\t  => ", build_types[build_type])
    exit(1)

# Gather command line arguments
platform_name = argv[1]
build_flag = argv[2]
docker_build_file = path.abspath(
    path.join("docker", platform_targets[platform_name]))

# Gather local system information
local_platform = uname()
build_start_time = datetime.now()

# Print startup message
print("+" + "-" * 78 + "+")
print("| QUIX IDE Project Build System" + " " * 48 + "|")
print("| " + " " * 76 + " |")
l = f"Local system: {local_platform[0]} {local_platform[2]} {local_platform[4]}"[
    :76]
print("| " + l + " " * (76 - len(l)) + " |")
l = f"Local datetime: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}"[:76]
print("| " + l + " " * (76 - len(l)) + " |")
print("|" + " " * 78 + "|")
l = f"Building the QUIX IDE Project for the platform: {platform_name}."[:76]
print("| " + l + " " * (76 - len(l)) + " |")
l = f"Build configuration is set to a {build_types[build_flag]}."[:76]
print("| " + l + " " * (76 - len(l)) + " |")
l = f"Using local Docker build file: {docker_build_file}"
while len(l) > 76:
    print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
    l = l[76:]
else:
    print("| " + l + " " * (76 - len(l)) + " |")
print("|" + " " * 78 + "|")
l = f"Build started at: {build_start_time.strftime('%Y-%m-%d %H:%M:%S')}"[:76]
print("| " + l + " " * (76 - len(l)) + " |")
print("+" + "-" * 78 + "+")

print()

print("+" + "-" * 78 + "+")
print("| Updating the local Docker image for the platform: " +
      platform_name + " " * 15 + "|")
print("|" + " " * 78 + "|")

# ==============================================================================
# BEGIN SECURITY: RUN LOCAL OS COMMAND
local_os_command = f"docker build -t quix-ide-{platform_name}:latest -f {docker_build_file} ."

l = f"Running the local OS command: \"{local_os_command}\""
while len(l) > 76:
    print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
    l = l[76:]
else:
    print("| " + l + " " * (76 - len(l)) + " |")
print("|" + " " * 78 + "|")


# Run the local OS command and capture the output in variable `output` capture
# stdout and stderr in the same variable
output = Popen(local_os_command, shell=True,
               stdout=PIPE, stderr=PIPE)

while True:
    stdout_ln = output.stdout.readline()
    stderr_ln = output.stderr.readline()

    if not stdout_ln and not stderr_ln:
        break

    if stdout_ln:
        l = stdout_ln.decode("utf-8").strip()
        while len(l) > 76:
            print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
            l = l[76:]
        else:
            print("| " + l + " " * (76 - len(l)) + " |")

    if stderr_ln:
        l = stderr_ln.decode("utf-8").strip()
        while len(l) > 76:
            print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
            l = l[76:]
        else:
            print("| " + l + " " * (76 - len(l)) + " |")

print("|" + " " * 78 + "|")

output.wait()

if output.returncode == 0:
    print("| Docker image updated successfully." + " " * 43 + "|")
else:
    l = f"Docker image update failed with error code: {output.returncode}"
    print("| " + l + " " * (76 - len(l)) + " |")
    build_end_time = datetime.now()
    l = f"Build completed at: {build_end_time.strftime('%Y-%m-%d %H:%M:%S')} take a total of {build_end_time - build_start_time}"
    while len(l) > 76:
        print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
        l = l[76:]
    else:
        print("| " + l + " " * (76 - len(l)) + " |")
    print("+" + "-" * 78 + "+")
    exit(2)

print("+" + "-" * 78 + "+")

# END SECURITY: RUN LOCAL OS COMMAND
# ==============================================================================

print()

# ==============================================================================
# BEGIN SECURITY: RUN LOCAL OS COMMAND

print("+" + "-" * 78 + "+")
l = f"Building the QUIX IDE Project for the platform: {platform_name}."[:76]
print("| " + l + " " * (76 - len(l)) + " |")
print("|" + " " * 78 + "|")

build_directory = path.abspath(
    path.join(getcwd(), "build", platform_name, build_type_dirs[build_flag]))

if not path.exists(build_directory):
    makedirs(build_directory)

local_os_command = f"docker run --rm -v {getcwd()}:/src -v {build_directory}:/build quix-ide-{platform_name}:latest make {build_flag}"
l = f"Running the local OS command: \"{local_os_command}\""
while len(l) > 76:
    print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
    l = l[76:]
else:
    print("| " + l + " " * (76 - len(l)) + " |")
print("|" + " " * 78 + "|")

# Run the local OS command and capture the output in variable `output` capture
# stdout and stderr in the same variable
output = Popen(local_os_command, shell=True,
               stdout=PIPE, stderr=PIPE)

while True:
    stdout_ln = output.stdout.readline()
    stderr_ln = output.stderr.readline()

    if not stdout_ln and not stderr_ln:
        break

    if stdout_ln:
        l = stdout_ln.decode("utf-8").strip()
        while len(l) > 76:
            print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
            l = l[76:]
        else:
            print("| " + l + " " * (76 - len(l)) + " |")

    if stderr_ln:
        l = stderr_ln.decode("utf-8").strip()
        while len(l) > 76:
            print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
            l = l[76:]
        else:
            print("| " + l + " " * (76 - len(l)) + " |")

print("|" + " " * 78 + "|")

output.wait()

if output.returncode == 0:
    print("| Build completed successfully." + " " * 47 + "|")
else:
    l = f"Build failed with error code: {output.returncode}"
    print("| " + l + " " * (76 - len(l)) + " |")
    build_end_time = datetime.now()
    l = f"Build completed at: {build_end_time.strftime('%Y-%m-%d %H:%M:%S')} take a total of {build_end_time - build_start_time}"
    while len(l) > 76:
        print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
        l = l[76:]
    else:
        print("| " + l + " " * (76 - len(l)) + " |")
    print("+" + "-" * 78 + "+")
    exit(3)


build_end_time = datetime.now()
l = f"Build completed at: {build_end_time.strftime('%Y-%m-%d %H:%M:%S')} take a total of {build_end_time - build_start_time}"
while len(l) > 76:
    print("| " + l[:76] + " " * (76 - len(l[:76])) + " |")
    l = l[76:]
else:
    print("| " + l + " " * (76 - len(l)) + " |")
print("+" + "-" * 78 + "+")
