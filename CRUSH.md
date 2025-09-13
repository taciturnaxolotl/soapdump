# CRUSH Development Guidelines

## Commands
- **Run parser**: `./transaction-parser.sh <logfile>`
- **Test parser**: `./transaction-parser.sh -s <logfile>` (summary mode)
- **Help**: `./transaction-parser.sh --help`

## Code Style
- **Language**: Bash scripting
- **Shebang**: Use `#!/usr/bin/env nix-shell` with dependencies
- **Formatting**: 
  - 4-space indentation
  - Functions use snake_case
  - Variables use UPPER_CASE
- **Structure**:
  - Clear function separation
  - Help documentation included
  - Error handling with exit codes
- **Dependencies**: gnugrep, gnused, coreutils (via nix-shell)

## Best Practices
- Always validate input files exist
- Use proper error messages and exit codes
- Include comprehensive help documentation
- Follow tab-separated output format for structured data
- Handle edge cases in XML parsing

## Naming Conventions
- Variables: UPPER_CASE
- Functions: snake_case
- Files: kebab-case.sh

## Error Handling
- Check file existence before processing
- Validate arguments
- Exit with appropriate codes (0 for success, 1 for error)