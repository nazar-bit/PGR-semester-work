#include "Camera.hpp"
#include "SceneGraph.hpp"


namespace vasylnaz {

	glm::vec3 pointDebugColor = glm::vec3(0.6f, 0.0f, 0.6f);


    void Camera::addCurve(std::unique_ptr<Curve> curve, RenderContext& rq) {
        curve->buildCurve(CURVE_PRECISION);
        rq.curves.push_back(curve.get());
        curves.push_back(std::move(curve));
    }


    void Camera::followNode(Node* node) {
        current_node = node;
    }


    void Camera::moveToViewPoint(ViewPoint& view_point) {
        auto points = std::vector<glm::vec3>();
        points.push_back(position);
        points.push_back(position);
        points.push_back(view_point.position);
        points.push_back(view_point.position);
        changeCurve(new Curve(points));

        target_before_rotation = target;
        up_before_rotation = up;
        // Target
        rotation = calculateRotation(target, view_point.target);

        // Up
        rotation_up = calculateRotation(up, view_point.up);
    }



    void Camera::changeCurve(Curve* curve) {
        current_curve = curve;
        curve_movement_start = -1;
        rotation = glm::quat(NAN_FL, NAN_FL, NAN_FL, NAN_FL);
        rotation_up = glm::quat(NAN_FL, NAN_FL, NAN_FL, NAN_FL);
    }



    void Camera::update() {
        // Check if follows node
        if (current_node != nullptr) {
            position = current_node->model_mat[3];
            target = glm::normalize(glm::vec3(current_node->model_mat[2]));
            target = glm::rotate(target, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            up = glm::normalize(glm::vec3(current_node->model_mat[1]));
            position += up*0.3f + target*0.7f;
            return;
        }
        // Check if moving along curve
        if (current_curve == nullptr) {
            return;
        }
        // Reset inner time
        if (curve_movement_start == -1) {
            curve_movement_start = GlobalTime;
        }
        // Calc new position along the curve
        auto new_pos = current_curve->moveAlong((GlobalTime - curve_movement_start) * curve_speed);
        // Check if curve ended
        if (glm::any(glm::isnan(new_pos))) {
            current_curve = nullptr;
            curve_movement_start = -1;
            rotation = glm::quat(NAN_FL, NAN_FL, NAN_FL, NAN_FL);
            rotation_up = glm::quat(NAN_FL, NAN_FL, NAN_FL, NAN_FL);
            return;
        }
        // Curve did not end
        else {
            // Check if to use default rotation
            if (glm::any(glm::isnan(rotation))) {
                glm::quat newRotation = calculateRotationCurve(position, new_pos); 
                target = newRotation * defaultBackward;
                up = newRotation * defaultUp;
            }
            // Use custom rotation
            else {
                auto t = (GlobalTime - curve_movement_start) * curve_speed;
                if (t < 1) {
                    auto cur_rot = glm::slerp(glm::quat(1.0f, glm::vec3(0.0f)), rotation, t);
                    target = cur_rot * target_before_rotation;
                    cur_rot = glm::slerp(glm::quat(1.0f, glm::vec3(0.0f)), rotation_up, t);
                    up = cur_rot * up_before_rotation;
                }
            }

            // Update position
            position = new_pos;
        }
    }



    void Camera::drawViewPoints(const ShaderProgram& shader_manager, const glm::mat4& view) {
        glBindVertexArray(vao);

        glUniformMatrix4fv(shader_manager.positionVM, 1, GL_FALSE, glm::value_ptr(view));

        glDrawArrays(GL_POINTS, 0, view_points.size());
        glBindVertexArray(0);
    }



    void Camera::init(RenderContext& render_context) {
        // ViewPoints
        std::vector<glm::vec3> points;

        // 1
        ViewPoint view_point = ViewPoint(
            glm::vec3(-4.2f, 2.0f, 1.5f),
            glm::normalize(glm::vec3(-glm::sqrt(2.0f)-0.2f, 0.5, -glm::sqrt(2.0f)+0.2f)),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        view_points.push_back(view_point);
        points.push_back(view_point.position);

        // 2
        view_point = ViewPoint(
            glm::vec3(-4.2f, 0.0f, 8.0f),
            glm::normalize(glm::vec3(-glm::sqrt(2.0f) - 0.5f, 0.5, glm::sqrt(2.0f) + 0.5f)),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        view_points.push_back(view_point);
        points.push_back(view_point.position);


        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(Attributes::POSITION);
        glVertexAttribPointer(Attributes::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(0);


        // Curves
        auto c_points = std::vector<glm::vec3>();
        c_points.push_back(glm::vec3(4.5f, 2.0f, 9.5f));
        c_points.push_back(glm::vec3(4.0f, 1.9f, 8.5f));
        c_points.push_back(glm::vec3(3.0f, 1.6f, 8.5f));
        c_points.push_back(glm::vec3(2.5f, 1.4f, 8.0f));
        c_points.push_back(glm::vec3(2.3f, 1.3f, 7.0f));

        c_points.push_back(glm::vec3(2.5f, 1.1f, 6.5f));
        c_points.push_back(glm::vec3(2.7f, 1.1f, 6.0f));
        c_points.push_back(glm::vec3(3.0f, 1.0f, 5.8f));
        c_points.push_back(glm::vec3(3.2f, 0.9f, 5.5f));

        c_points.push_back(glm::vec3(3.0f, 0.9f, 5.2f));
        c_points.push_back(glm::vec3(2.8f, 0.9f, 5.0f));

        c_points.push_back(glm::vec3(2.5f, 0.8f, 5.0f));
        c_points.push_back(glm::vec3(2.2f, 0.7f, 4.8f));
        c_points.push_back(glm::vec3(1.8f, 0.7f, 4.5f));
        c_points.push_back(glm::vec3(1.3f, 0.6f, 4.2f));
        c_points.push_back(glm::vec3(0.5f, 0.6f, 4.0f));
        c_points.push_back(glm::vec3(-0.5f, 0.5f, 3.8f));
        c_points.push_back(glm::vec3(-1.0f, 0.4f, 3.5f));
        c_points.push_back(glm::vec3(-1.5f, 0.3f, 3.0f));

        c_points.push_back(glm::vec3(-1.7f, 0.3f, 2.8f));
        c_points.push_back(glm::vec3(-1.9f, 0.3f, 2.6f));
        c_points.push_back(glm::vec3(-2.5f, 0.3f, 2.1f));
        c_points.push_back(glm::vec3(-3.0f, 0.3f, 2.0f));
        auto curve = std::unique_ptr<Curve>(new Curve(c_points));
        addCurve(std::move(curve), render_context);

    }
}