using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;
using ArtemisAndXNA_tryingOutHeaven.Components;
using Microsoft.Xna.Framework;

namespace ArtemisAndXNA_tryingOutHeaven.Systems
{
	class MovementSystem : EntityProcessingSystem
	{
		ComponentMapper<Transform> m_transformMapper;
		ComponentMapper<Velocity> m_velocityMapper;

		public MovementSystem()
			: base(typeof(Transform), typeof(Velocity))
		{
			m_transformMapper = null;
			m_velocityMapper = null;
		}

		public override void Initialize()
		{
			m_transformMapper = new ComponentMapper<Transform>(world);
			m_velocityMapper = new ComponentMapper<Velocity>(world);
		}

		public override void Process(Entity e)
		{
			Transform transform = m_transformMapper.Get(e);
			Velocity velocity = m_velocityMapper.Get(e);

			float dt = (float)(world.Delta) * 1000.0f;

			transform.position.X += velocity.velocity.X * dt;
			transform.position.Y += velocity.velocity.Y * dt;
		}
	}
}
