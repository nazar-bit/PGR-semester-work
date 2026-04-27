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
        target = view_point.target;
        up = view_point.up;
        auto points = std::vector<glm::vec3>();
        points.push_back(position);
        points.push_back(position);
        points.push_back(view_point.position);
        points.push_back(view_point.position);
        changeCurve(new Curve(points));
    }



    void Camera::changeCurve(Curve* curve) {
        current_curve = curve;
        curve_movement_start = -1;
    }



    void Camera::update(float global_time) {
        if (current_node != nullptr) {
            position = current_node->model_mat[3];
            position.y += 0.3f;
            position.x -= 0.7f;
            return;
        }

        if (current_curve == nullptr) {
            return;
        }

        if (curve_movement_start == -1) {
            curve_movement_start = global_time;
        }

        auto new_pos = current_curve->moveAlong((global_time - curve_movement_start) * curve_speed);
        if (glm::any(glm::isnan(new_pos))) {
            current_curve = nullptr;
            curve_movement_start = -1;
            return;
        }
        else {
            position = new_pos;
        }
    }



    void Camera::drawViewPoints(const ShaderProgram& shader_manager, const glm::mat4& view) {
        glBindVertexArray(vao);

        glUniformMatrix4fv(shader_manager.positionVM, 1, GL_FALSE, glm::value_ptr(view));

        glDrawArrays(GL_POINTS, 0, view_points.size());
        glBindVertexArray(0);
    }



    void Camera::initViewPoints() {
        std::vector<glm::vec3> points;

        ViewPoint view_point = ViewPoint(
            glm::vec3(10.0f, 2.0f, 3.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
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
    }
}