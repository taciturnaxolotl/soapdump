# CRUSH Development Guidelines

## Commands
- **Build with nix**: `nix build`
- **Run parser**: `./result/bin/transaction-parser <logfile>`
- **Test parser**: `./result/bin/transaction-parser -s <logfile>` (summary mode)
- **Help**: `./result/bin/transaction-parser --help`
- **Build with clang**: `clang++ -std=c++17 -O3 -o transaction-parser src/transaction-parser.cpp`

## Code Style
- **Language**: C++17
- **Formatting**: 
  - 4-space indentation
  - Functions use camelCase
  - Classes use PascalCase
  - Variables use camelCase
- **Structure**:
  - Clear function separation
  - Help documentation included
  - Error handling with return codes
- **Dependencies**: Standard C++ libraries only

## Build System
- **Nix Flake**: For reproducible builds and development environment
- **CMake**: For cross-platform build configuration
- **Compiler**: Clang++ with C++17 standard

## Best Practices
- Always validate input files exist
- Use proper error messages and return codes
- Include comprehensive help documentation
- Follow pipe-separated output format for structured data
- Handle edge cases in XML parsing with regex
- Use STL containers and algorithms for performance

## Naming Conventions
- Variables: camelCase
- Functions: camelCase
- Classes: PascalCase
- Files: kebab-case.cpp

## Error Handling
- Check file existence before processing
- Validate arguments
- Return appropriate codes (0 for success, 1 for error)
- Use try/catch blocks for exception handling