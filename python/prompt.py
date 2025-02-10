import sys
import readline
import os
import subprocess
import json
import re
import time

HISTORY_FILE = os.path.expanduser("~/.custom_terminal_history")
command_history = []

# Load history from file if it exists
if os.path.exists(HISTORY_FILE):
    with open(HISTORY_FILE, "r") as f:
        command_history = [line.strip() for line in f if line.strip()]
        for command in command_history:
            readline.add_history(command)

# Enable tab completion
def completer(text, state):
    commands = ["exit", "get", "help", "history", "pull", "push", "quit", "set", "exec", "test"]
    targets = {"ABC", "XYZ", "123"}  # Example targets
    buffer = readline.get_line_buffer()
    words = buffer.split()
    
    if not words:
        options = commands
    elif len(words) == 1 and not buffer.endswith(" "):
        options = [cmd for cmd in commands if cmd.startswith(text)]
    elif words[0] in {"get", "set", "push", "pull"} and (len(words) == 1 or (len(words) == 2 and buffer.endswith(" "))):
        options = [t for t in targets if t.startswith(text)]
    else:
        options = []
    
    return options[state] if state < len(options) else None

readline.set_completer(completer)
readline.parse_and_bind("tab: complete")

def yellow(text):
    return f"\033[33m{text}\033[0m"

def green(text):
    return f"\033[32m{text}\033[0m"

def red(text):
    return f"\033[31m{text}\033[0m"

def orange(text):
    return f"\033[38;5;214m{text}\033[0m"

def blue(text):
    return f"\033[34m{text}\033[0m"

def bold_yellow(text):
    return f"\033[1;33m{text}\033[0m"

def bold_green(text):
    return f"\033[1;32m{text}\033[0m"

def bold_red(text):
    return f"\033[1;31m{text}\033[0m"

def show_help():
    commands = {
        "exit": "Exit the terminal",
        "get": "Retrieve a value. Usage: get <TARGET>",
        "help": "Show available commands",
        "history": "Show command history. Usage: history [clear]",
        "pull": "Pull an item from the stack. Usage: pull <TARGET>",
        "push": "Push an item to the stack. Usage: push <TARGET> [forced]",
        "quit": "Exit the terminal",
        "set": "Set a value. Usage: set <TARGET> <VALUE>",
        "exec": "Execute a Linux command. Usage: exec <COMMAND>",
        "test": "Run automated tests from a JSON file. Usage: test <FILE>",
    }
    print("Available commands:")
    for cmd in sorted(commands):
        print(f"  {cmd}: {commands[cmd]}")

def run_tests(file_path):
    try:
        with open(file_path, "r") as f:
            tests = json.load(f)
    except Exception as e:
        print(red(f"Error loading test file: {e}"))
        return

    overall_result = True
    for test in tests:
        command = test.get("command", "")
        pattern = test.get("pattern", "")
        stop_if_false = test.get("stopIfFailure", True)
        no_retries = test.get("noRetries", 1)
        print('-'*50)
        print(f"{yellow('Executing')}: {command}")
        for attempt in range(no_retries):
            result = subprocess.run(command, shell=True, text=True, capture_output=True)
            output = result.stdout.strip() + "\n" + result.stderr.strip()
            print(f"{blue('Output')}:\n{output.strip()}")

            if re.search(pattern, output, re.MULTILINE):
                print(green(f"Passed: Pattern '{pattern}' found in output"))
                break
            else:
                print(red(f"Failed: Pattern '{pattern}' not found in output (Attempt {attempt + 1}/{no_retries})"))
                if attempt + 1 < no_retries:
                    time.sleep(1)
        else:
            overall_result = False
            if stop_if_false:
                break

    print(bold_green("All tests passed") if overall_result else bold_red("Some tests failed"))

def process_command(command):
    parts = command.split()
    if not parts:
        return
    cmd = parts[0].lower()
    args = parts[1:]
    targets = {"ABC", "XYZ", "123"}  # Example list of valid targets
    
    match cmd:
        case "push" if len(args) >= 1:
            target = args[0]
            if target in targets:
                forced = "forced" in args[1:]
                print(f"Pushing {target} {'with force' if forced else ''}")
            else:
                print("Invalid target.")
        case "pull" if len(args) == 1:
            target = args[0]
            if target in targets:
                print(f"Pulling {target}")
            else:
                print("Invalid target.")
        case "get" if len(args) == 1:
            target = args[0]
            if target in targets:
                print(f"Getting value for {target}")
            else:
                print("Invalid target.")
        case "set" if len(args) == 2:
            target, value = args
            if target in targets:
                print(f"Setting {target} to {value}")
            else:
                print("Invalid target.")
        case "history" if not args:
            print("Current session history:")
            for cmd in command_history:
                print(cmd)
        case "history" if args and args[0] == "clear":
            command_history.clear()
            open(HISTORY_FILE, "w").close()
            print("History cleared.")
        case "exec" if args:
            try:
                result = subprocess.run(" ".join(args), shell=True, executable="/bin/bash", text=True, capture_output=True)
                output = result.stdout + result.stderr
                print(output.strip())
                print(f"Exit Code: {result.returncode}")
            except Exception as e:
                print(f"Error executing command: {e}")
        case "test" if args:
            run_tests(args[0])
        case _:
            print("Unknown or incorrect command usage.")

def main():
    try:
        while True:
            command = input("$ ")
            if command.strip():
                if not command_history or command_history[-1] != command:
                    command_history.append(command)
            if command.lower() in {"exit", "quit"}:
                print("Exiting terminal...")
                break
            elif command.lower() == "help":
                show_help()
            else:
                process_command(command)
    finally:
        with open(HISTORY_FILE, "w") as f:
            f.write("\n".join(command_history) + "\n")

if __name__ == "__main__":
    main()