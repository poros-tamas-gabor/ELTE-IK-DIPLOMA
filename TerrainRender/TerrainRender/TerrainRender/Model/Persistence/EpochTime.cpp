#include <iostream>
#include "EpochTime.h"


EpochTime::EpochTime(long long sec, long long nsec) 
{
    m_seconds = sec;
    if (nsec >= NSEC_PER_SEC) {
        m_seconds += nsec / NSEC_PER_SEC;
        m_nanoseconds = nsec % NSEC_PER_SEC;
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
    if (nsec >= NSEC_PER_SEC) {
        sec += nsec / NSEC_PER_SEC;
        nsec %= NSEC_PER_SEC;
    }
    return EpochTime(sec, nsec);
}

bool EpochTime::operator==(const EpochTime& other) const
{
    return this->m_seconds == other.m_seconds && this->m_nanoseconds == other.m_nanoseconds;
}

EpochTime EpochTime::operator-(const EpochTime& other) const 
{
    long long sec = m_seconds - other.m_seconds;
    long long nsec = m_nanoseconds - other.m_nanoseconds;
    if (nsec < 0) {
        sec -= 1;
        nsec += NSEC_PER_SEC;
    }
    return EpochTime(sec, nsec);
}
double EpochTime::diffInMillis(const EpochTime& other) const
{
    EpochTime diff = *this - other;
    double ellapsedMillisec = diff.m_seconds * MSEC_PER_SEC + (double)diff.m_nanoseconds / NSEC_PER_MSEC;
    return ellapsedMillisec;
}

EpochTime EpochTime::AddMilliSeconds(double millisecond) const
{
    long long sec = m_seconds + long long(millisecond / MSEC_PER_SEC);
    millisecond = fmod(millisecond, MSEC_PER_SEC);
    long long nsec = m_nanoseconds + long long(millisecond * NSEC_PER_MSEC);
    if (nsec >= NSEC_PER_SEC) {
        sec += nsec / NSEC_PER_SEC;
        nsec %= NSEC_PER_SEC;
    }
    return EpochTime(sec, nsec);
}


// getters
long long EpochTime::getSeconds() const { return m_seconds; }
long long EpochTime::getNanoseconds() const { return m_nanoseconds; }

void EpochTime::setSeconds(long long sec) { m_seconds = sec; };
void EpochTime::setNanoseconds(long long nsec) { m_nanoseconds = nsec; };
