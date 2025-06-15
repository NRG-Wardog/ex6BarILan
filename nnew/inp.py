import os
import subprocess

INPUT_FILE = "input.txt"
INPUTS_DIR = "inputs"
os.makedirs(INPUTS_DIR, exist_ok=True)

with open(INPUT_FILE, "r", encoding="utf-8") as f:
    content = f.read()

raw_inputs = content.split("Input\n")[1:]

input_paths = []
for i, raw in enumerate(raw_inputs, 1):
    lines = raw.strip().splitlines()
    clean_lines = []
    for line in lines:
        if line.startswith("Correct Output") or line.startswith("Your choice:"):
            break
        clean_lines.append(line.strip())
    input_path = os.path.join(INPUTS_DIR, f"input_{i:02}.txt")
    with open(input_path, "w", encoding="utf-8") as f:
        f.write("\n".join(clean_lines))
    input_paths.append(input_path)

test_work = 0

for i, input_path in enumerate(input_paths, 1):
    my_output = f"outputmy_{i:02}.txt"
    ref_output = f"outputref_{i:02}.txt"

    print(f"Running test {i:02}...")

    subprocess.run(f"ex6_window < {input_path} > {my_output}", shell=True)
    subprocess.run(f"ex6 < {input_path} > {ref_output}", shell=True)

    result = subprocess.run(["diff", my_output, ref_output], capture_output=True, text=True)

    if result.stdout == "":
        print(f"Test {i:02}: ✅ Passed")
        test_work += 1
    else:
        print(f"Test {i:02}: ❌ Failed")
        print(result.stdout)

print("Cleaning up...")
for path in input_paths:
    os.remove(path)

for i in range(1, len(input_paths) + 1):
    os.remove(f"outputmy_{i:02}.txt")
    os.remove(f"outputref_{i:02}.txt")

os.rmdir(INPUTS_DIR)
print(f"Done ✅ with {test_work} test work")
