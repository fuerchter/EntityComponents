#ifndef MESSAGETYPE
#define MESSAGETYPE

namespace ec
{
	//RENAME??
	enum MessageType
	{
		MInt,
		MFloat,
		MString,
		MIntRect,
		MColor,
		MVector2f,
		MVector2u,
		MSprite,
		
		MInsertEntity,
		MEntity,
		MEntityAttributes,
		MEntityEnd,
		MRemoveEntity,
		MComponent,
	};
}

#endif