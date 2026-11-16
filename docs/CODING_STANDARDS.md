# CODING STANDARDS

## Naming Conventions
- **Namespaces**: `kow::` (Kenya Open World)
- **Types**: PascalCase (e.g., `EntityManager`)
- **Variables**: camelCase (e.g., `entityCount`)
- **Constants**: UPPER_SNAKE_CASE (e.g., `MAX_ENTITIES`)
- **Functions**: camelCase (e.g., `createEntity`)
- **Interfaces**: `I` prefix (e.g., `IRenderer`)
- **Abstract classes**: `I` prefix or `Base` suffix

## Code Style
- **Indentation**: 4 spaces, no tabs
- **Line length**: 100 characters max
- **Braces**: Allman style (brace on new line)
- **Comments**: `//` for short, `/** */` for documentation

## C++20 Usage
- Use `std::optional`, `std::variant`, `std::expected`
- Concepts for template constraints
- `consteval`, `constexpr` where appropriate
- Designated initializers
- `std::span` for non-owning views

## Memory Management
- **Unique_ptr**: Single ownership
- **Shared_ptr**: Shared ownership (rare)
- **Raw pointers**: Non-owning, for observers
- **No malloc/free**: Use custom allocators

## Error Handling
- Result<T> for recoverable errors
- Assert for invariants
- Log errors, don't throw

## Testing
- Google Test framework
- Unit tests for all modules
- Test fixtures for setup/teardown
- Mock objects for dependencies

## Documentation
- Doxygen for API docs
- Function-level comments for complex logic
- Module README for subsystems