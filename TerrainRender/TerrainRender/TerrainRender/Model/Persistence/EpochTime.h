#ifndef EPOCH_TIME_H
#define EPOCH_TIME_H

class EpochTime {
public:
    EpochTime(long long sec = 0, long long nsec = 0);

    // arithmetic operators
    EpochTime operator+(const EpochTime& other) const;
    EpochTime operator-(const EpochTime& other) const;
    bool operator==(const EpochTime& other) const;

    double diffInMillis(const EpochTime& other) const;

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

