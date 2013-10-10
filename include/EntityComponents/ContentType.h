#ifndef CONTENTTYPE
#define CONTENTTYPE

namespace ec
{
	//RENAME??
	enum ContentType
	{
		MInt,
		MFloat,
		MString,
		MIntRect,
		MColor,
		MVector2f,
		MVector2u,
		MSprite,
		MSpriteCollection, //Am i even using this?
		MEntityManager,
		
		MInsertEntity,
		MEntity,
		MEntityAttributes,
		MEntityEnd,
		MRemoveEntity,
		MComponent,
		
		MInsertSprite,
		MChangeSprite,
		MRemoveSprite
	};
}

#endif