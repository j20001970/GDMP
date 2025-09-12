import os
import urllib.request
import zipfile
import argparse

# https://stackoverflow.com/questions/6861323/download-and-unzip-file-with-python

DOWNLOAD_URL = "https://github.com/godotengine/godot/releases/download/{version}-stable/Godot_v{version}-stable_{os}.{arch}.zip"


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--current-os", help="The GitHub action runner being used.", required=True)
    parser.add_argument(
        "--godot-version", help="The version of Godot to download.", required=True)

    args = parser.parse_args()

    current_os = args.current_os
    current_arch = ""
    if current_os == "windows-latest":
        current_os = "win64"
        current_arch = "exe"
    elif current_os == "ubuntu-latest":
        current_os = "linux"
        current_arch = "x86_64"
    elif current_os == "macos-latest":
        import sys
        sys.exit(-1)  # Not supported for now

    dl_url = DOWNLOAD_URL.format(
        version=args.godot_version, os=current_os, arch=current_arch)

    filehandle, _ = urllib.request.urlretrieve(dl_url)
    zip_file_object = zipfile.ZipFile(filehandle, "r")
    first_file = zip_file_object.namelist()[0]
    file = zip_file_object.open(first_file)

    with open("godot", "wb") as f:
        f.write(file.read())

    if current_os == "linux":
        os.chmod("godot", 0o777)
