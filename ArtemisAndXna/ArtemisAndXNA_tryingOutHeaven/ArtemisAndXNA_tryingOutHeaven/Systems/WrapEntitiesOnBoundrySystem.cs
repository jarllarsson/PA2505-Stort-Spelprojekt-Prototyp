using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Artemis;
using ArtemisAndXNA_tryingOutHeaven.Components;

namespace ArtemisAndXNA_tryingOutHeaven.Systems
{
	class WrapEntitiesOnBoundrySystem : EntityProcessingSystem
	{
		ComponentMapper<Transform> m_transformMapper;

		public WrapEntitiesOnBoundrySystem()
			: base(typeof(Transform))
		{
		}

		public override void Initialize()
		{
			m_transformMapper = new ComponentMapper<Transform>(world);
		}

		public override void Process(Entity e)
		{
			Transform transform = m_transformMapper.Get(e);

			if (transform.position.X < 0.0f)
			{
				transform.position.X = 1680.0f;
			}
			else if (transform.position.X > 1680.0f)
			{
				transform.position.X = 0.0f;
			}
			else if (transform.position.Y < 0.0f)
			{
				transform.position.Y = 1050.0f;
			}
			else if (transform.position.Y > 1050.0f)
			{
				transform.position.Y = 0.0f;
			}
		}

	}
}
