import os
import subprocess
import sys

# script may call al the executables that start with a given input and print the output of them
# [EXAMPLE] ./runner.py Test

def getExecutables( prefix:str ) -> list:
    # finds exxecutablees in the dir that start with prefix

    dir_path = "./"
    executables = []

    for file in os.listdir(dir_path):
        file_path = os.path.join(dir_path, file)
        if file.startswith(prefix) and os.access(file_path, os.X_OK):
            #print(file)
            executables.append(file_path)

    return executables

def runExecutables( executables:list )->None:

    # validate input
    if not executables:
            print(f"[ERROR] No executables were given.")
            return

    # iterate over each file
    for exe in executables:
        output = f"Running: {exe} -> "
        try:
            subprocess.run([exe], capture_output=True, text=True, check=True)
            output += "success"
        except subprocess.CalledProcessError:
            output += "error on execution"
        except Exception as e:
            output += f"[ERROR] {e}"

        print(output)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python runner.py <prefix>")
        sys.exit(1)

    # Get the prefix from the command-line argument
    prefix = sys.argv[1]
    ls = getExecutables(prefix)
    runExecutables(ls)
