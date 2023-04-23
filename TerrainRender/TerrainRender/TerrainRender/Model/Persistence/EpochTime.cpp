#include <iostream>
#include "EpochTime.h"


static const long long nsecPerSec    = 1000000000L;
static const long long msecPerSec    = 1000L;
static const long long nsecPermSec   = 1000000L;

EpochTime::EpochTime(long long sec, long long nsec) 
{
    m_seconds = sec;
    if (nsec >= nsecPerSec) {
        m_seconds += nsec / nsecPerSec;
        m_nanoseconds = nsec % nsecPerSec;
    }
    else
    {
        m_nanoseconds = nsec;
    }
}

    // arithmetic operators
EpochTime EpochTime::operator+(const EpochTime& other) const 
{
    long long sec = m_seconds + other.m_seconds;
    long long nsec = m_nanoseconds + other.m_nanoseconds;
    if (nsec >= nsecPerSec) {
        sec += nsec / nsecPerSec;
        nsec %= nsecPerSec;
    }
    return EpochTime(sec, nsec);
}

EpochTime EpochTime::operator-(const EpochTime& other) const 
{
    long long sec = m_seconds - other.m_seconds;
    long long nsec = m_nanoseconds - other.m_nanoseconds;
    if (nsec < 0) {
        sec -= 1;
        nsec += nsecPerSec;
    }
    return EpochTime(sec, nsec);
}
double EpochTime::diffInMillis(const EpochTime& other) const
{
    EpochTime diff = *this - other;
    double ellapsedMillisec = diff.m_seconds * msecPerSec + (double)diff.m_nanoseconds / nsecPermSec;
    return ellapsedMillisec;
}

EpochTime EpochTime::AddMilliSeconds(double millisecond) const
{
    long long sec = m_seconds + long long(millisecond / msecPerSec);
    millisecond = fmod(millisecond, msecPerSec);
    long long nsec = m_nanoseconds + long long(millisecond * nsecPermSec);
    if (nsec >= nsecPerSec) {
        sec += nsec / nsecPerSec;
        nsec %= nsecPerSec;
    }
    return EpochTime(sec, nsec);
}


// getters
long long EpochTime::getSeconds() const { return m_seconds; }
long long EpochTime::getNanoseconds() const { return m_nanoseconds; }

void EpochTime::setSeconds(long long sec) { m_seconds = sec; };
void EpochTime::setNanoseconds(long long nsec) { m_nanoseconds = nsec; };
