#ifndef EGG_BINARY_H
#define EGG_BINARY_H

template <class T>
class IBinary {
public:
    class Bin {};
    virtual void SetBinaryInner(Bin &) = 0;
    virtual void GetBinaryInner(Bin *) const = 0;
    virtual size_t GetBinarySize() = 0;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) = 0;

    static const char *GetBinaryType();
    int GetVersion();

    void GetBinary(void *) const;

    void SetBinary(const void *);

    void SetBinaryBlend(const void *, const void *, f32);
};

#endif
