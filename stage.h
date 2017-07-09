#pragma once

#include <tari/geometry.h>
#include <tari/datastructures.h>

#include "mugenspritefilereader.h"

// documentation at http://www.elecbyte.com/mugendocs-11b1/bgs.html

typedef struct {
	char mName[1024];
	char mDisplayName[1024];
	char mVersionDate[100];
	char mMugenVersion[100];
	char mAuthor[100];
} StageInfo;

typedef struct {
	Position mStartPosition;
	double mBoundLeft;
	double mBoundRight;
	double mBoundHigh;
	double mBoundLow;
	
	double mTension;
	double mTensionHigh;
	double mTensionLow;

	double mVerticalFollow;
	double mFloorTension;

	int mOverdrawHigh;
	int mOverdrawLow;
	int mCutHigh;
	int mCutLow;

	double mStartZoom;
	double mZoomOut;
	double mZoomIn;

} StageCamera;

typedef struct {
	Position mP1Start;
	Position mP2Start;

	int mP1Facing;
	int mP2Facing;

	double mLeftBound;
	double mRightBound;

} StagePlayerInfo;

typedef struct {
	double mScreenLeft;
	double mScreenRight;
} StageBound;

typedef struct {
	double mZOffset;
	int mZOffsetLink;
	int mAutoturn;
	int mResetBG;
	Position mLocalCoordinates;
	Vector3D mScale;

} StageStageInfo;

typedef struct {
	int mIntensity;
	Vector3D mColor;
	double mScaleY;
	Vector3DI mFadeRange;
	double mXShear;

} StageShadow;

typedef struct {
	int mReflect;
} StageReflection;

typedef struct {
	char mBGMusic[1024];
	int mBGVolume;
} StageMusic;

typedef struct {
	char mSpritePath[1024];
	int mDebugBG;

} StageBackgroundDefinition;

typedef enum {
	STAGE_BACKGROUND_STATIC,
	STAGE_BACKGROUND_ANIMATED,
} StageBackgroundType;

typedef struct {
	StageBackgroundType mType;
	Vector3DI mSpriteNo;
	int mLayerNo;
	Position mStart;
	Position mDelta;
	int mMask;
	Vector3DI mTile;
	Vector3DI mTileSpacing;
	int mListPosition;
} StageBackgroundElement;

typedef struct {
	StageInfo mInfo;
	StageCamera mCamera;
	StagePlayerInfo mPlayerInfo;
	StageBound mBound;
	StageStageInfo mStageInfo;
	StageShadow mShadow;
	StageReflection mReflection;
	StageMusic mMusic;
	StageBackgroundDefinition mBackgroundDefinition;

	List mBackgroundElements;

	MugenSpriteFile mSprites;
} Stage;

extern Stage gStage;

void loadStageFromMugenDefinition(char* mPath);