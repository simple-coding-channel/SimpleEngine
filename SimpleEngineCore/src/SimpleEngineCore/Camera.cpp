#include "SimpleEngineCore/Camera.hpp"

#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SimpleEngine {
    Camera::Camera(const glm::vec3& position,
        const glm::vec3& rotation,
        const ProjectionMode projection_mode)
        : m_position(position)
        , m_rotation(rotation)
        , m_projection_mode(projection_mode)
    {
        update_view_matrix();
        update_projection_matrix();
    }

    const glm::mat4& Camera::get_view_matrix()
    {
        if (m_update_view_matrix)
        {
            update_view_matrix();
            m_update_view_matrix = false;
        }
        return m_view_matrix;
    }

    void Camera::update_view_matrix()
    {
        const float roll_in_radians  = glm::radians(m_rotation.x);
        const float pitch_in_radians = glm::radians(m_rotation.y);
        const float yaw_in_radians   = glm::radians(m_rotation.z);

        const glm::mat3 rotate_matrix_x(1, 0, 0,
            0, cos(roll_in_radians), sin(roll_in_radians),
            0, -sin(roll_in_radians), cos(roll_in_radians));

        const glm::mat3 rotate_matrix_y(cos(pitch_in_radians), 0, -sin(pitch_in_radians),
            0, 1, 0,
            sin(pitch_in_radians), 0, cos(pitch_in_radians));

        const glm::mat3 rotate_matrix_z(cos(yaw_in_radians), sin(yaw_in_radians), 0,
            -sin(yaw_in_radians), cos(yaw_in_radians), 0,
            0, 0, 1);

        const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;
        m_direction = glm::normalize(euler_rotate_matrix * s_world_forward);
        m_right = glm::normalize(euler_rotate_matrix * s_world_right);
        m_up = glm::cross(m_right, m_direction);

        m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);
    }

    void Camera::update_projection_matrix()
    {
        if (m_projection_mode == ProjectionMode::Perspective)
        {
            m_projection_matrix = glm::perspective(glm::radians(m_field_of_view), m_viewport_width / m_viewport_height, m_near_clip_plane, m_far_clip_plane);
        }
        else
        {
            float r = 2;
            float t = 2;
            float f = 100;
            float n = 0.1f;
            m_projection_matrix = glm::mat4(1 / r, 0, 0, 0,
                                            0, 1 / t, 0, 0,
                                            0, 0, -2 / (f - n), 0,
                                            0, 0, (-f - n) / (f - n), 1);
        }
    }

    void Camera::set_position(const glm::vec3& position)
    {
        m_position = position;
        m_update_view_matrix = true;
    }

    void Camera::set_rotation(const glm::vec3& rotation)
    {
        m_rotation = rotation;
        m_update_view_matrix = true;
    }

    void Camera::set_position_rotation(const glm::vec3& position, const glm::vec3& rotation)
    {
        m_position = position;
        m_rotation = rotation;
        m_update_view_matrix = true;
    }

    void Camera::set_projection_mode(const ProjectionMode projection_mode)
    {
        m_projection_mode = projection_mode;
        update_projection_matrix();
    }

    void Camera::set_far_clip_plane(const float far)
    {
        m_far_clip_plane = far;
        update_projection_matrix();
    }

    void Camera::set_near_clip_plane(const float near)
    {
        m_near_clip_plane = near;
        update_projection_matrix();
    }

    void Camera::set_viewport_size(const float width, const float height)
    {
        m_viewport_width = width;
        m_viewport_height = height;
        update_projection_matrix();
    }

    void Camera::set_field_of_view(const float fov)
    {
        m_field_of_view = fov;
        update_projection_matrix();
    }

    void Camera::move_forward(const float delta)
    {
        m_position += m_direction * delta;
        m_update_view_matrix = true;
    }

    void Camera::move_right(const float delta)
    {
        m_position += m_right * delta;
        m_update_view_matrix = true;
    }

    void Camera::move_up(const float delta)
    {
        m_position += s_world_up * delta;
        m_update_view_matrix = true;
    }

    void Camera::add_movement_and_rotation(const glm::vec3& movement_delta,
                                             const glm::vec3& rotation_delta)
    {
        m_position += m_direction * movement_delta.x;
        m_position += m_right     * movement_delta.y;
        m_position += m_up        * movement_delta.z;
        m_rotation += rotation_delta;
        m_update_view_matrix = true;
    }

}