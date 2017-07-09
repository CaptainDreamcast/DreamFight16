#pragma once

#include <tari/datastructures.h>
#include <tari/animation.h>

typedef struct {
	List mPassiveHitboxes; // contain Colliders
	List mAttackHitboxes;
	int mGroupNumber;
	int mSpriteNumber;
	Vector3D mDelta;
	Duration mDuration;
	int mIsFlippingHorizontally;
	int mIsFlippingVertically;
} MugenAnimationStep;

typedef struct {
	int mLoopStart;
	Vector mSteps;
} MugenAnimation;

typedef struct {
	IntMap mAnimations;

} MugenAnimations;

MugenAnimations loadMugenAnimationFile(char* tPath);