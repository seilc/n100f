#include "xCommon.h"
#include "xEntDrive.h"

void xEntDriveInit(xEntDrive* drv, xEnt* driven)
{
    drv->flags = 0;
    drv->driven = driven;
    drv->driver = NULL;
    drv->s = 0.0f;
    drv->tm = 0.0f;
    drv->tmr = 0.0f;
    drv->odriver = NULL;
    drv->os = 0.0f;
    drv->otm = 0.0f;
    drv->otmr = 0.0f;
}

void xEntDriveMount(xEntDrive* drv, xEnt* driver, F32 mt)
{
    if (driver == drv->odriver && drv->os) {
        drv->driver = driver;
        drv->s = drv->os;
        drv->tm = mt;
        drv->tmr = mt * (1.0f - drv->s);
        drv->odriver = NULL;
        drv->os = 0.0f;
        drv->otm = 0.0f;
        drv->otmr = 0.0f;
    } else {
        drv->driver = driver;
        drv->s = 0.0f;
        drv->tm = mt;
        drv->tmr = mt;
    }

    if (drv->flags & 0x1) {
        xVec3 euler;
        xMat3x3GetEuler((xMat3x3*)&drv->driver->frame->mat, &euler);
        drv->yaw = euler.x;
    }

    xVec3Copy(&drv->q, &drv->driven->frame->mat.pos);
    xMat4x3ToLocal(&drv->p, &drv->driver->frame->mat, &drv->q);
}

void xEntDriveDismount(xEntDrive* drv, F32 dmt)
{
    xEnt* pDriver = drv->driver;
    if (!pDriver) {
        return;
    }

    drv->odriver = pDriver;
    drv->os = drv->s;
    drv->otm = dmt;
    drv->otmr = dmt * drv->os;
    drv->driver = NULL;
    drv->s = 0.0f;
    drv->tm = 0.0f;
    drv->tmr = 0.0f;

    xVec3Copy(&drv->q, &drv->driven->frame->mat.pos);
    xMat4x3ToLocal(&drv->op, &drv->odriver->frame->mat, &drv->q);
}

void xEntDriveUpdate(xEntDrive* drv, xScene*, F32 dt)
{
    if (!drv->odriver && !drv->driver) {
        return;
    }

    if (drv->otmr > 0.0f) {
        drv->otmr -= dt;
        if (drv->otmr <= 0.0f) {
            drv->os = 0.0f;
            drv->otmr = 0.0f;
        } else {
            drv->os = drv->otmr / drv->otm;
        }
    }

    if (drv->tmr > 0.0f) {
        drv->tmr -= dt;
        if (drv->tmr <= 0.0f) {
            drv->s = 1.0f;
            drv->tmr = 0.0f;
        } else {
            drv->s = 1.0f - drv->tmr / drv->tm;
        }
    }

    if (!drv->os && !drv->s) {
        return;
    }

    if (drv->s && (drv->flags & 0x1)) {
        xVec3 euler;
        xMat3x3 rot;
        xMat3x3GetEuler((xMat3x3*)&drv->driver->frame->mat, &euler);
        xMat3x3RotY(&rot, drv->s * (euler.x - drv->yaw));
        xMat3x3Mul((xMat3x3*)&drv->driven->frame->mat, (xMat3x3*)&drv->driven->frame->mat, &rot);
        drv->yaw = euler.x;
    }

    xVec3 newq;

    if (drv->os) {
        xMat4x3ToWorld(&newq, &drv->odriver->frame->mat, &drv->op);
        xVec3Sub(&drv->driven->frame->dpos, &newq, &drv->q);
        xVec3SMulBy(&drv->driven->frame->dpos, drv->os);
        xVec3AddTo(&drv->driven->frame->mat.pos, &drv->driven->frame->dpos);
    }

    if (drv->s) {
        xMat4x3ToWorld(&newq, &drv->driver->frame->mat, &drv->p);
        xVec3Sub(&drv->driven->frame->dpos, &newq, &drv->q);
        xVec3SMulBy(&drv->driven->frame->dpos, drv->s);
        xVec3AddTo(&drv->driven->frame->mat.pos, &drv->driven->frame->dpos);
    }

    xVec3Copy(&drv->q, &drv->driven->frame->mat.pos);

    if (drv->os) {
        xMat4x3ToLocal(&drv->op, &drv->odriver->frame->mat, &drv->q);
    }

    if (drv->s) {
        xMat4x3ToLocal(&drv->p, &drv->driver->frame->mat, &drv->q);
    }
}
