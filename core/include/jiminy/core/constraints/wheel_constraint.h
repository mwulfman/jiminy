#ifndef JIMINY_WHEEL_CONSTRAINT_H
#define JIMINY_WHEEL_CONSTRAINT_H

#include <memory>

#include "jiminy/core/macros.h"
#include "jiminy/core/types.h"
#include "jiminy/core/constraints/abstract_constraint.h"


namespace jiminy
{
    class Model;

    /// \brief Class constraining a wheel to roll without slipping on a flat plane.
    ///
    /// \details Given a frame to represent the wheel center, this class constrains it to move
    ///          like it were rolling without slipping on a flat (not necessarily level) surface.
    class JIMINY_DLLAPI WheelConstraint : public AbstractConstraintTpl<WheelConstraint>
    {
    public:
        DISABLE_COPY(WheelConstraint)

        auto shared_from_this() { return shared_from(this); }

    public:
        /// \param[in] frameName Name of the frame representing the center of the wheel.
        /// \param[in] wheelRadius Radius of the wheel (in m).
        /// \param[in] groundNormal Normal to the ground in world frame as a unit vector.
        /// \param[in] wheelAxis Axis of the wheel, in the local frame.
        WheelConstraint(const std::string & frameName,
                        const float64_t & wheelRadius,
                        const Eigen::Vector3d & groundNormal,
                        const Eigen::Vector3d & wheelAxis);
        virtual ~WheelConstraint() = default;

        const std::string & getFrameName() const;
        const frameIndex_t & getFrameIdx() const;

        void setReferenceTransform(const pinocchio::SE3 & transformRef);
        const pinocchio::SE3 & getReferenceTransform() const;

        virtual hresult_t reset(const Eigen::VectorXd & /* q */,
                                const Eigen::VectorXd & /* v */) override final;

        virtual hresult_t computeJacobianAndDrift(const Eigen::VectorXd & q,
                                                  const Eigen::VectorXd & v) override final;

    private:
        /// \brief Name of the frame on which the constraint operates.
        std::string frameName_;
        /// \brief Corresponding frame index.
        frameIndex_t frameIdx_;
        /// \brief Wheel radius.
        float64_t radius_;
        /// \brief Ground normal, world frame.
        Eigen::Vector3d normal_;
        /// \brief Wheel axis, local frame.
        Eigen::Vector3d axis_;
        /// \brief Skew matrix of wheel axis, in world frame, scaled by radius.
        Eigen::Matrix3d skewRadius_;
        /// \brief Derivative of skew matrix of wheel axis, in world frame, scaled by radius.
        Eigen::Matrix3d dskewRadius_;
        /// \brief Reference pose of the frame to enforce.
        pinocchio::SE3 transformRef_;
        /// \brief Stores full frame jacobian in world.
        Matrix6Xd frameJacobian_;
    };
}

#endif  // end of JIMINY_WHEEL_CONSTRAINT_H
