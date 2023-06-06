#ifndef EPOCH_TIME_H
#define EPOCH_TIME_H

///////////////////////////////////////////////////////////////////////////////
// EpochTime.h
// ============
//
// The EpochTime class is a representation of time with nanosecond precision, defined by two long long integer values: m_seconds and m_nanoseconds. 
// It provides basic arithmetic operators for addition and subtraction of EpochTime objects as well as comparison operators.
// It includes methods for converting the time difference between two EpochTime objects to milliseconds and for adding a specified number of milliseconds to an EpochTime object.
// 
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

class EpochTime {

public:
    static const long long NSEC_PER_SEC = 1000000000L;
    static const long long MSEC_PER_SEC = 1000L;
    static const long long NSEC_PER_MSEC = 1000000L;

public:
    EpochTime(long long sec = 0, long long nsec = 0);

    // arithmetic operators
    EpochTime operator+(const EpochTime& other) const;
    EpochTime operator-(const EpochTime& other) const;
    bool operator==(const EpochTime& other) const;

    double DiffInMilliSec(const EpochTime& other) const;
    EpochTime AddMilliSeconds(double millisecond) const;

    // getters
    long long getSeconds() const;
    long long getNanoseconds() const;

    //setters
    void setSeconds(long long);
    void setNanoseconds(long long);

private:
    long long m_seconds;
    long long m_nanoseconds;
};

#endif

