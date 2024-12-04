#!/bin/bash

# Define the commands to test, including more comprehensive scenarios
TEST_COMMANDS=(
    # Basic commands
    "echo 'Hello, World!'"
    "pwd"
    "ls -la"

    # Redirection tests
    "echo 'Testing output redirection' > test_output.txt"
    "cat < test_output.txt"
    "echo 'Appending to file' >> test_output.txt"
    "cat test_output.txt"
    "grep 'Testing' < test_output.txt > grep_output.txt"
    "cat grep_output.txt"

    # Pipe tests
    "ls -la | grep 'test_output.txt'"
    "cat test_output.txt | wc -l"
    "echo -e 'Line 1\nLine 2\nLine 3' | wc -l"

    # Background execution
    "sleep 1 &"
    "echo 'This should appear before sleep finishes'"

    # Special characters
    "echo \$HOME"
    "echo \"Double quotes test\""
    "echo 'Single quotes test'"
    "echo Backslashes \\\\ test"

    # Here-document (heredoc)
    "cat << EOF
This is a test of heredoc
EOF"

    # Subshells and command substitution
    "echo \$(echo 'Nested command')"

    # Variable assignment (if supported)
    "MYVAR='Test variable'"
    "echo \$MYVAR"

    # Error handling
    "invalid_command"
    "ls nonexistentfile"

    # Clean up
    "rm test_output.txt grep_output.txt"

    # Exit
    "exit"
)

# Create a temporary file to store test commands
TEST_FILE="test_commands.txt"
OUTPUT_FILE="minishell_test_results.txt"
VALGRIND_LOG="valgrind_log.txt"

# Write test commands to the temporary file
printf "%s\n" "${TEST_COMMANDS[@]}" > "$TEST_FILE"

# Run the minishell program with input from the test file, save output to a file, and print to terminal
echo "Running minishell with test commands..."
./minishell < "$TEST_FILE" | tee "$OUTPUT_FILE"

# Run Valgrind to check for memory leaks and append output to the output file and print to terminal
echo "Running Valgrind..."
valgrind --leak-check=full --track-origins=yes --log-file="$VALGRIND_LOG" ./minishell < "$TEST_FILE" | tee -a "$OUTPUT_FILE"

# Append Valgrind log to the output file and print to terminal
echo -e "\nValgrind Output:" | tee -a "$OUTPUT_FILE"
cat "$VALGRIND_LOG" | tee -a "$OUTPUT_FILE"

# Notify user that all outputs are saved in the file
echo "All test outputs and Valgrind results have been saved to $OUTPUT_FILE"

# Clean up temporary files
rm "$TEST_FILE" "$VALGRIND_LOG"
