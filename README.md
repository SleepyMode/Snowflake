# Snowflake
C++ header-only Implementation of Twitter's Snowflake algorithm.

## Terminology
- Instances are referred to as "Machine Identifiers" (.machineId)
- Sequences are referred to as "Worker Identifiers" (.workerId)

## Usage
Sample usage can be found in test.cc. Make sure to initialise the library with
SnowflakeId::initialise prior to using (w/ epoch and machine identifier).
