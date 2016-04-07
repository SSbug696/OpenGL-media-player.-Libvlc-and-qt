#include "Anaglyph.h"
#include <QGL>
#include <math.h>
#include <iostream>

Anaglyph::Anaglyph(float Convergence,
                   float EyeSeparation,
                   float AspectRatio,
                   float FOV,
                   float NearClippingDistance,
                   float FarClippingDistance
                        ){
    _flag = 0;
    _mConvergence            = Convergence;
    _mEyeSeparation          = EyeSeparation;
    _mAspectRatio            = AspectRatio;
    _mFOV                    = FOV * 3.14 / 180.0f;
    _mNearClippingDistance   = NearClippingDistance;
    _mFarClippingDistance    = FarClippingDistance;
}


void Anaglyph::ApplyLeftFrustum() {

    if(_flag & 2) {
        top     = _ltop;
        bottom  = _lbottom;
        left    = _lleft;
        right   = _lright;
    } else {
        top     = _mNearClippingDistance * tan(_mFOV/2);
        bottom  = -top;

        float a = _mAspectRatio * tan(_mFOV/2) * _mConvergence;

        float b = a - _mEyeSeparation/2;
        float c = a + _mEyeSeparation/2;

        left    = -b * _mNearClippingDistance/_mConvergence;
        right   =  c * _mNearClippingDistance/_mConvergence;

        _ltop   = top;
        _lbottom= bottom;
        _lright = right;
        _lleft  = left;

        _flag++;
    }

    // Set the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top,
              _mNearClippingDistance, _mFarClippingDistance);

    // Displace the world to right
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(_mEyeSeparation/2, 0.0f, 0.0f);
}


void Anaglyph::ApplyRightFrustum()
{
    if(_flag & 2) {
        top     = _rtop;
        bottom  = _rbottom;
        left    = _rleft;
        right   = _rright;
    } else {
        top     = _mNearClippingDistance * tan(_mFOV/2);
        bottom  = -top;

        float a = _mAspectRatio * tan(_mFOV/2) * _mConvergence;

        float b = a - _mEyeSeparation/2;
        float c = a + _mEyeSeparation/2;

        left    =  -c * _mNearClippingDistance/_mConvergence;
        right   =   b * _mNearClippingDistance/_mConvergence;

        _rtop   = top;
        _rbottom= bottom;
        _rright = right;
        _rleft  = left;

        _flag++;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top,
              _mNearClippingDistance, _mFarClippingDistance);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-_mEyeSeparation/2, 0.0f, 0.0f);
}
