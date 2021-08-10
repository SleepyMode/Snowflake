
#pragma once

#if defined(_WIN32)
#	include <Windows.h>
#endif

#include <mutex>
#include <cstdint>
#include <type_traits>
#include <chrono>

class SnowflakeLockBase
{
public:
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

class SnowflakeLockStdMutex : public std::mutex, SnowflakeLockBase
{
public:
	virtual void lock() override
	{
		std::mutex::lock();
	}

	virtual void unlock() override
	{
		std::mutex::unlock();
	}
};

#if defined(_WIN32)
class SnowflakeLockCriticalSection : public SnowflakeLockBase
{
public:
	SnowflakeLockCriticalSection()
	{
		criticalSection = { 0 };
		InitializeCriticalSection(&criticalSection);
	}

	~SnowflakeLockCriticalSection()
	{
		DeleteCriticalSection(&criticalSection);
	}

	virtual void lock() override
	{
		EnterCriticalSection(&criticalSection);
	}

	virtual void unlock() override
	{
		LeaveCriticalSection(&criticalSection);
	}

private:
	CRITICAL_SECTION criticalSection;
};
#endif

class SnowflakeScopeLock
{
public:
	SnowflakeScopeLock(SnowflakeLockBase& lock) : lock(lock)
	{
		lock.lock();
	}

	~SnowflakeScopeLock()
	{
		lock.unlock();
	}

private:
	SnowflakeLockBase& lock;
};

class SnowflakeId
{
public:
#if defined(_WIN32)
	using SnowflakeLockType = SnowflakeLockCriticalSection;
#else
	using SnowflakeLockType = SnowflakeLockStdMutex;
#endif

	std::uint64_t timestamp : 41;
	std::uint64_t machineId : 10;
	std::uint64_t workerId : 12;

	std::uint64_t implode()
	{
		std::uint64_t value = timestamp;
		value <<= 10;
		value += machineId;
		value <<= 12;
		value += workerId;

		return value;
	}

	static void initialize(std::uint64_t epoch, std::uint64_t machineId)
	{
		cfgEpoch = epoch;
		cfgMachineId = machineId;
		currentWorkerId = 0;
		lock = SnowflakeLockType();
	}

	static SnowflakeId generate()
	{
		SnowflakeScopeLock scopeLock(lock);
		if (currentWorkerId >= 4095) // [0, 4095]
			currentWorkerId = 0;
		else
			++currentWorkerId;

		return SnowflakeId{
			.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - cfgEpoch,
			.machineId = cfgMachineId,
			.workerId = currentWorkerId
		};
	}

	friend bool operator==(const SnowflakeId& lhs, const SnowflakeId& rhs);
	friend bool operator!=(const SnowflakeId& lhs, const SnowflakeId& rhs);

private:
	static inline std::uint64_t cfgEpoch;
	static inline std::uint64_t cfgMachineId;
	static inline std::uint64_t currentWorkerId;
	static inline SnowflakeLockType lock;
};

inline bool operator==(const SnowflakeId& lhs, const SnowflakeId& rhs)
{
	return ((lhs.timestamp == rhs.timestamp) and (lhs.machineId == rhs.machineId)
		and (lhs.workerId == rhs.workerId));
}

inline bool operator!=(const SnowflakeId& lhs, const SnowflakeId& rhs)
{
	return ((lhs.timestamp != rhs.timestamp) or (lhs.machineId != rhs.machineId)
		or (lhs.workerId != rhs.workerId));
}
