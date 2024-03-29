#pragma once
#include "I3D.h"

#include <EASTL/vector.h>
#include <EASTL/string.h>
#include <EASTL/shared_ptr.h>
namespace ea = eastl;

#include <glm/ext.hpp>

enum I3D_FRAME_FLAGS : uint32_t {
    FRMFLAGS_ON             = (1 << 1),
    FRMFLAGS_MAT_DIRTY      = (1 << 2),
    FRMFLAGS_POS_DIRTY      = (1 << 5),
    FRMFLAGS_ROT_DIRTY      = (1 << 3), 
    FRMFLAGS_SCALE_DIRTY    = (1 << 4)
};

class I3D_frame {
public:
    I3D_frame(I3D_driver* driver) :
        _flags(FRMFLAGS_ON | FRMFLAGS_MAT_DIRTY),
        _driver(driver) {
    }

    I3D_FRAME_TYPE getFrameType() const { return _type; }
    void duplicate(I3D_frame* src);

    void setFrameFlags(uint32_t flags) { _flags = flags; }
    uint32_t getFrameFlags() const { return _flags; }

    I3D_frame* getParent() { return _parent; }
    const auto& getChildren() { return _children; }

    void addChild(ea::shared_ptr<I3D_frame> child);
    void removeChild(ea::shared_ptr<I3D_frame> child);

    bool isOn() const { return _flags & FRMFLAGS_ON; }
    void setOn(bool on);

    const ea::string& getName() const { return _name; }
    void setName(ea::string& name) { _name = name; }

    const glm::mat4& getLocalMatrix();
    const glm::mat4& getMatrix();
    void setMatrix(const glm::mat4& mat);

    const glm::vec3& getPos() const;
    void setPos(glm::vec3& pos);

    const glm::quat& getRot() const;
    void setRot(const glm::quat& rot);

    const glm::vec3& getScale() const;
    void setScale(const glm::vec3& scale);
protected:
    void propagateDirty();
    I3D_driver* _driver{ nullptr };
    I3D_FRAME_TYPE _type{};
    uint32_t _flags{};
    I3D_frame* _parent{ nullptr };
    ea::vector<ea::shared_ptr<I3D_frame>> _children{};
    ea::string _name{};
    glm::mat4 _worldMatrix{}; // resulting world matrix = _matrix * parnent's matrix
    glm::mat4 _matrix{}; // resulting local matrix = scale * rot * transl.
    glm::vec3 _pos{};
    glm::quat _rot{};
    glm::vec3 _scale{};
};