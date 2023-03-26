#include <iostream>
#include "EpochTime.h"


static const unsigned nsecPerSec    = 1000000000;
static const unsigned msecPerSec    = 1000;
static const unsigned nsecPermSec   = 1000000;

EpochTime::EpochTime(long long sec, long long nsec) : m_seconds(sec), m_nanoseconds(nsec) {}

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
