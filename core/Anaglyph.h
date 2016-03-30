#ifndef ANAGLYPH_H
#define ANAGLYPH_H


class Anaglyph
{
public:
    Anaglyph(float, float, float, float, float, float);
    void ApplyLeftFrustum();
    void ApplyRightFrustum();

private:
    float _mConvergence;
    float _mEyeSeparation;
    float _mAspectRatio;
    float _mFOV;
    float _mNearClippingDistance;
    float _mFarClippingDistance;


    float _rtop, _rbottom, _rleft, _rright,
            _ltop, _lbottom, _lleft, _lright;

    float top, bottom, left, right;

    short _flag;
};

#endif // ANAGLYPH_H
