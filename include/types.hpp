

struct Color
{
    unsigned char r, g, b;

    unsigned char toGray() const
    {
        return static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
    }
};
