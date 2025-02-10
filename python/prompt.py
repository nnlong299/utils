import sys
import readline
import os
import subprocess

HISTORY_FILE = os.path.expanduser("~/.custom_terminal_history")
command_history = []

# Load history from file if it exists
if os.path.exists(HISTORY_FILE):
    with open(HISTORY_FILE, "r") as f:
        command_history = [line.strip() for line in f if line.strip()]
        for command in command_history:
            readline.add_history(command)

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
    }
    print("Available commands:")
    for cmd in sorted(commands):
        print(f"  {cmd}: {commands[cmd]}")

def complete(text, state):
    commands = ["exit", "quit", "help", "push", "pull", "get", "set", "history", "exec"]
    targets = ["ABC", "XYZ", "123"]
    buffer = readline.get_line_buffer().strip()
    words = buffer.split()
    
    if not buffer:  # Suggest commands if buffer is empty
        options = commands
    elif len(words) == 1:  # Suggest commands if only one word is typed
        options = [cmd for cmd in commands if cmd.startswith(text)]
    elif len(words) == 2 and words[0] in {"push", "pull", "get", "set"}:  # Suggest targets
        options = [tgt for tgt in targets if tgt.startswith(text)]
    else:
        options = []
    
    return options[state] if state < len(options) else None

readline.set_completer(complete)
readline.parse_and_bind("tab: complete")  # Enable tab completion

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
            open(HISTORY_FILE, "w").close()  # Clear history file on disk
            print("History cleared.")
        case "exec" if args:
            try:
                result = subprocess.run(" ".join(args), shell=True, executable="/bin/bash", text=True)
                if result.returncode:
                    print(f"Exit Code: {result.returncode}")
            except Exception as e:
                print(f"Error executing command: {e}")
        case _:
            print("Unknown or incorrect command usage.")

def get_input():
    try:
        command = input("$ ")  # Prompt user for input
        if command.strip():
            if not command_history or command_history[-1] != command:  # Avoid consecutive duplicates
                command_history.append(command)
        return command
    except KeyboardInterrupt:
        print("\nUse 'exit' or 'quit' to leave.")
        return ""
    except EOFError:
        print("\nExiting terminal...")
        return "exit"

def main():
    try:
        while True:
            command = get_input()
            if command.lower() in {"exit", "quit"}:  # Allow user to exit
                print("Exiting terminal...")
                break
            elif command.lower() == "help":
                show_help()
            else:
                process_command(command)
    finally:
        # Save command history when exiting
        with open(HISTORY_FILE, "w") as f:
            f.write("\n".join(command_history) + "\n")

if __name__ == "__main__":
    main()
