# Cat & Grep Implementation

**Developer:** LikimiaD

---

This repository contains custom implementations of the `cat` and `grep` commands, written in C programming language.

#### Project Structure

```
.
├── Makefile
├── cat
│   ├── custom_cat.c           # Source code for the cat implementation
│   ├── generate.py            # Script for generating special ASCII characters
│   ├── penetration_test.py    # Script for testing consistency with the original cat and grep versions in Linux
│   └── tests                  # Test files for penetration_test.py
│       ├── TEST-FILE_001
│       ├── ...
│       └── TEST-FILE_005
└── grep
    ├── custom_grep.c          # Source code for the grep implementation
    ├── patterns               # Test files for the -f flag in penetration_test.py
    │   ├── PATTERN-FILE_001
    │   ├── ...
    │   └── PATTERN-FILE_003
    ├── penetration_test.py    # Script for testing consistency with the original grep version in Linux
    └── tests                  # Test files for penetration_test.py
        ├── TEST-FILE_001
        ├── ...
        └── TEST-FILE_004
```

#### Features

- **penetration_test.py:** Tests the program's consistency with the original versions of `cat` and `grep` in Linux. The script captures the output of both our program and the original and then conducts a character-by-character comparison.

- **generate.py:** A script to generate special ASCII characters, ranging from 10 to 127, used for testing the `-v` flag in `cat`.

#### How to Use

1. Build the project:
   ```
   make
   ```

2. Run `custom_cat`:
   ```
   ./custom_cat [path_to_file]
   ```

3. Run `custom_grep`:
   ```
   ./custom_grep [pattern] [path_to_file]
   ```

4. Generate special characters for `cat`:
   ```
   python3 generate.py
   ```

5. Execute penetration test for `cat` or `grep`:
   ```
   python3 cat/penetration_test.py
   python3 grep/penetration_test.py
   ```

#### Cat Description

The `cat` version presented here is written in the C programming language and offers the following features:

**Supported Options:**

    -b, --number-nonblank: Number the non-empty lines.
    
    -e: Equivalent to -vE.
    
    -E, --show-ends: Display a $ at the end of each line.
    
    -n, --number: Number all output lines.
    
    -s, --squeeze-blank: Suppress repeated empty lines.
    
    -t: Equivalent to -vT.
    
    -T, --show-tabs: Display ^I instead of TAB characters.
    
    -v, --show-nonprinting: Use ^ and M- notations, except for LF and TAB.

**Key Functions:**

    struct_fill: Initializes the mode structure to its default values.

    struct_change_value: Alters a value within the mode structure based on a passed character.

    check_mode: Checks and sets the mode based on given arguments.

    handle_space_lines: Processes and removes consecutive blank lines.

    handle_tabs: Replaces TAB characters with ^I.

    handle_line_numbers: Displays line numbers depending on the active modes.

    print_file: Reads and outputs the file's content considering the selected modes.

Regarding the provided C code for the `grep` command, the tool emulates a subset of the Unix `grep` functionalities. Here's a comprehensive analysis:

**Data Structures:**

Mode: A structure used to store the flags or options of the grep command. The structure has integer members corresponding to the supported options (e, i, v, c, l, n, h, s, f, o).

**Main Function:**

The primary role initializes the Mode structure, identifies the options and the pattern from the command line arguments, and processes the input files accordingly based on the given options.

**Function Descriptions:**

    fill_struct: Initializes the Mode structure to zero.
    
    print_struct: Displays the Mode structure (for debugging purposes).
    
    input_flags: Recognizes and sets the flags based on the command-line arguments.
    
    switch_flags: Activates the relevant flags in the Mode structure according to the flag character provided.
    
    read_patterns_from_file: Retrieves patterns from a file when used with the -f flag.
    
    find_word: Locates a word within a given buffer. The -i flag ensures a case-insensitive search.
    
    find_word_handle_o: A refined search function tailored for the -o flag, displaying only the matching segments of the input.
    
    custom_to_lower: Converts a character to its lowercase form.
    
    print_file: Reads, processes, and outputs the lines from the input file based on the established flags and pattern.

**Supported Options:**

    -e: Present in the code but seemingly unused.
    
    -i: Enables case-insensitive searching.
    
    -v: Inverts the match to select non-matching lines.
    
    -c: Only outputs the count of matching lines.
    
    -l: Displays only the names of files with the matching pattern.
    
    -n: Prefixes each output line with its line number from the input file.
    
    -h: Omits the filenames from the output.
    
    -s: Suppresses error messages about non-existent or unreadable files.
    
    -f: Reads patterns from a file, with each pattern on a new line.
    
    -o: Only showcases the section of a matching line that corresponds with the pattern.

**Limitations:**

The error messages and prompts may differ from the original `grep`.
    
The code does not encompass all grep options.