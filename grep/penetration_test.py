import os
import re
import random
import subprocess
from filecmp import cmp
from itertools import chain, combinations

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def limited_combinations(lst, r=2):
    return list(chain.from_iterable(combinations(lst, i) for i in range(1, r + 1)))

your_grep = "./s21_grep"
original_grep = "/bin/grep"
test_files_dir = "tests/"
pattern_files_dir = "patterns/"
remove_files = "rm -rf *.txt s21_grep"
compile_gcc = "make"
test_files = [f"TEST-FILE_{str(i).zfill(3)}" for i in range(1, 3)]
pattern_files = [f"PATTERN-FILE_{str(i).zfill(3)}" for i in range(1, 3)]

flags = ['e', 'i', 'v', 'c', 'l', 'n', 'h', 's', 'f', 'o']

all_words = set()
word_pattern = re.compile(r'\w+')

for test_file in test_files:
    with open(os.path.join(test_files_dir, test_file), 'r') as f:
        content = f.read()
        words = word_pattern.findall(content)
        all_words.update(words)
        
num_words_to_select = min(2, len(all_words))
patterns = random.sample(list(all_words), num_words_to_select)

for file in [your_grep, original_grep] + [os.path.join(test_files_dir, tf) for tf in test_files]:
    if not os.path.exists(file):
        print(f"File {file} does not exist!")
        exit(1)

flag_combinations = limited_combinations(flags)

for flags_combination in flag_combinations:
    if (('e' in flags_combination or 'f' in flags_combination) and len(flags_combination) != 1) or (('v' in flags_combination) and ('o' in flags_combination)) or (('o' in flags_combination) and ('c' in flags_combination) or (('l' in flags_combination) and len(flags_combination) > 1)):
        continue
    for test_file in test_files:
        test_file_path = os.path.join(test_files_dir, test_file)
        for pattern in patterns:
            for grep_cmd, output_file in [(your_grep, "your_output.txt"), (original_grep, "original_output.txt")]:
                if 'f' in flags_combination:
                    pattern_file_path = os.path.join(pattern_files_dir, f"PATTERN-FILE_{patterns.index(pattern) + 1:03d}")
                    cmd = f"{grep_cmd} -{''.join(flags_combination)} {pattern_file_path} {test_file_path} > {output_file}"
                    print(cmd)
                else:
                    cmd = f"{grep_cmd} -{''.join(flags_combination)} {pattern} {test_file_path} > {output_file}"
                subprocess.run(cmd, shell=True)
                if (cmd[0:len(your_grep)] == your_grep):
                    subprocess.run("valgrind --tool=memcheck --leak-check=yes " + cmd, shell=True)

            if cmp("your_output.txt", "original_output.txt"):
                print(bcolors.OKGREEN + f"TEST PASSED: {test_file} with flags -{''.join(flags_combination)} and pattern {pattern}" + bcolors.ENDC)
            else:
                print(bcolors.FAIL + f"TEST FAILED: {test_file} with flags -{''.join(flags_combination)} and pattern {pattern}" + bcolors.ENDC)
                exit(1)

subprocess.run(remove_files, shell=True)
