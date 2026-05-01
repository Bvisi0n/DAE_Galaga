#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"

namespace dae::core
{
	Transform::Transform( GameObject* owner, const TransformDescriptor& descriptor )
		: m_owner{ owner }
		, m_localMatrix{ 1.F }
		, m_worldMatrix{ 1.F }
		, m_worldPosition{ descriptor.localPosition }
		, m_localPosition{ descriptor.localPosition }
		, m_localScale{ descriptor.localScale }
		, m_localRotation{ descriptor.localRotation }
	{}

	void Transform::SetLocalPosition( const glm::vec3& pos )
	{
		m_localPosition = pos;
		SetDirty();
	}

	void Transform::SetLocalPosition( float xPos, float yPos, float zPos )
	{
		SetLocalPosition( glm::vec3{ xPos, yPos, zPos } );
	}

	const glm::vec3& Transform::GetLocalPosition() const
	{
		return m_localPosition;
	}

	void Transform::SetLocalRotation( float angleInRadians )
	{
		m_localRotation = angleInRadians;
		SetDirty();
	}

	float Transform::GetLocalRotation() const
	{
		return m_localRotation;
	}

	void Transform::SetLocalScale( const glm::vec3& scale )
	{
		m_localScale = scale;
		SetDirty();
	}

	void Transform::SetLocalScale( float xScale, float yScale, float zScale )
	{
		SetLocalScale( glm::vec3{ xScale, yScale, zScale } );
	}

	const glm::vec3& Transform::GetLocalScale() const
	{
		return m_localScale;
	}

	const glm::mat4& Transform::GetWorldMatrix() const
	{
		return m_worldMatrix;
	}

	const glm::vec3& Transform::GetWorldPosition() const
	{
		return m_worldPosition;
	}

	void Transform::UpdateWorldMatrix( const glm::mat4& parentWorldMatrix )
	{
		UpdateLocalMatrix();
		m_worldMatrix = parentWorldMatrix * m_localMatrix;
		m_worldPosition = glm::vec3( m_worldMatrix[ 3 ] );

		for ( const auto& child : m_owner->GetChildren() )
		{
			child->GetTransform().UpdateWorldMatrix( m_worldMatrix );
		}
	}

	void Transform::SetDirty()
	{
		if ( m_isDirty )
		{
			return;
		}

		m_isDirty = true;
		for ( const auto& child : m_owner->GetChildren() )
		{
			child->GetTransform().SetDirty();
		}
	}

	void Transform::UpdateLocalMatrix()
	{
		if ( !m_isDirty )
		{
			return;
		}

		m_localMatrix = glm::translate( glm::mat4( 1.F ), m_localPosition );
		m_localMatrix = glm::rotate( m_localMatrix, m_localRotation, { 0, 0, 1 } );
		m_localMatrix = glm::scale( m_localMatrix, m_localScale );

		m_isDirty = false;
	}
}
