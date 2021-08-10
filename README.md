# Snowflake
C++ header-only Implementation of Twitter's Snowflake algorithm.

## Terminology
- Instances are referred to as "Machine Identifiers" (.machineId)
- Sequences are referred to as "Worker Identifiers" (.workerId)

## Usage
Sample usage can be found in test.cc. Make sure to initialize the library with
SnowflakeId::initialize prior to using (w/ epoch and machine identifier).

Example:
```cpp
int main(int argc, char* argv[], char* envp[])
{
	SnowflakeId::initialize(0, 50);
	SnowflakeId identifier = SnowflakeId::generate();
	std::cout << identifier.implode() << std::endl;
}
```
