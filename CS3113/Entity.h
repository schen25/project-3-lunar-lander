#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum Action    { PLAYER_FALLING, PLAYER_ACCELERATING, WATER_MOVING, GRASS_MOVING, CLOUD_MOVING, NO_ACTION };
enum EntityStatus { ACTIVE, INACTIVE };
enum EntityType   { PLAYER, GRASS, WATER, CLOUD, NONE };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D   mTexture;
    TextureType mTextureType;
    Vector2     mSpriteSheetDimensions;

    std::map<Action, std::vector<int>>      mAnimationAtlas;
    std::vector<int>                        mAnimationIndices;
    Action                                  mAction;
    int                                     mFrameSpeed;

    int   mCurrentFrameIndex = 0;
    float mAnimationTime     = 0.0f;

    int   mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;
    bool mHasCollidedWaterTop = false;
    bool mHasCollidedGrassOrCloudTop = false;

    bool mIsUsingJetpack = false;
    float mJetpackPower = 0.0f;
    float mFuel = 0.0f;
    float mFuelConsumption = 75.0f;

    EntityStatus mEntityStatus = ACTIVE;
    EntityType   mEntityType;

    bool isColliding(Entity *other) const;
    void checkCollisionY(const std::vector<Entity *>& collidableEntities, int collisionCheckCount);
    void checkCollisionX(const std::vector<Entity *>&, int collisionCheckCount);
    void resetColliderFlags()
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void animate(float deltaTime);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 50;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;
    static constexpr float ACCELERATION_OF_GRAVITY = 10.0f;

    Entity();
    Entity(
        Vector2     position,
        Vector2     scale,
        const char *textureFilepath,
        EntityType  entityType
    );
    Entity(
        Vector2                               position,
        Vector2                               scale,
        const char                           *textureFilepath,
        TextureType                           textureType,
        Vector2                               spriteSheetDimensions,
        std::map<Action, std::vector<int>> animationAtlas,
        EntityType                            entityType
    );
    ~Entity();

    void update(float deltaTime, const std::vector<Entity *>& collidableEntities, int collisionCheckCount);
    void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void activate()   { mEntityStatus = ACTIVE;    }
    void deactivate() { mEntityStatus = INACTIVE;  }
    void displayCollider();

    bool isActive() { return mEntityStatus == ACTIVE; }

    void useJetpack(){mIsUsingJetpack = true; mAction = PLAYER_ACCELERATING;    }
    void fall()      {mIsUsingJetpack = false; mAction = PLAYER_FALLING;        }
    void grassMove() {mAction = GRASS_MOVING;}
    void waterMove() {mAction = WATER_MOVING;}
    void cloudMove() {mAction = CLOUD_MOVING;}
    void rotateLeft() { mAngle = std::max(-90.0f, mAngle - 30.0f);          }
    void rotateRight() { mAngle = std::min(90.0f, mAngle + 30.0f);          }

    void resetMovement() { mMovement = { 1.0f, 0.0f }; }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getAcceleration()          const { return mAcceleration;          }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mColliderDimensions;    }
    Vector2     getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    Texture2D   getTexture()               const { return mTexture;               }
    TextureType getTextureType()           const { return mTextureType;           }
    Action   getAction()             const { return mAction;             }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    float       getJetpackPower()          const { return mJetpackPower;          }
    float       getFuel()                  const { return mFuel;                  }
    int         getSpeed()                 const { return mSpeed;                 }
    float       getAngle()                 const { return mAngle;                 }

    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }
    bool isCollidingLeft () const { return mIsCollidingLeft; }
    bool isCollidingRight() const { return mIsCollidingRight; }
    bool hasCollidedWaterTop() const { return mHasCollidedWaterTop; }
    bool HasCollidedGrassOrCloudTop() const { return mHasCollidedGrassOrCloudTop;}

    std::map<Action, std::vector<int>> getAnimationAtlas() const { return mAnimationAtlas; }

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setMovement(Vector2 newMovement)
        { mMovement = newMovement;                 }
    void setVelocity(Vector2 newVelocity)
        { mVelocity = newVelocity;                 }
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;         }
    void setScale(Vector2 newScale)
        { mScale = newScale;                       }
    void setTexture(const char *textureFilepath)
        { mTexture = LoadTexture(textureFilepath); }
    void setColliderDimensions(Vector2 newDimensions)
        { mColliderDimensions = newDimensions;     }
    void setSpriteSheetDimensions(Vector2 newDimensions)
        { mSpriteSheetDimensions = newDimensions;  }
    void setSpeed(int newSpeed)
        { mSpeed = newSpeed;                       }
    void setFrameSpeed(int newSpeed)
        { mFrameSpeed = newSpeed;                  }
    void setAngle(float newAngle)
        { mAngle = newAngle;                       }
    void setJetpackPower(float newJetpackPower)
        { mJetpackPower = newJetpackPower;         }
    void setFuel(float newFuel)
        { mFuel = newFuel;                         }
    void setEntityType(EntityType entityType)
        { mEntityType = entityType;                }
};

#endif // ENTITY_H