#pragma once


struct	PostProcessData;
struct	RenderBufferData;


/**
* RenderBuffer's attachment type.
* Only use RENDERBUFFER_TYPE_NONE on initialisation and reset only.
*/
enum RenderBufferType : uint {
	RENDERBUFFER_TYPE_NONE						= 0x00, // On initialisation only.
	RENDERBUFFER_TYPE_DEPTH_ATTACHMENT			= 0x01,
	RENDERBUFFER_TYPE_COLOUR_ATTACHMENT			= 0x02,
	RENDERBUFFER_TYPE_DEPTH_STENCIL_ATTACHMENT	= 0x03
};


/**
* RenderBufferCreationInfo data structure.
* Specify the information required to generate a RenderBuffer in OpenGL.
*
* @param [REQUIRED] (String) name - Name to be given to this renderbuffer.
* @param [REQUIRED] (RenderBufferType) type - The to be constructed renderbuffer's type.
*
* Below specifies the default internal format when different types are specified:
* [Type] RENDERBUFFER_TYPE_DEPTH_ATTACHMENT			= GL_DEPTH_COMPONENT32F.
* [Type] RENDERBUFFER_TYPE_DEPTH_STENCIL_ATTACHMENT = GL_DEPTH24_STENCIL8.
* [Type] RENDERBUFFER_TYPE_COLOUR_ATTACHMENT		= GL_RGBA8.
*/
struct RenderBufferCreationInfo {

	String				Name;
	RenderBufferType	Type;

};


/**
* Gravity's OffScreenBuffer data structure.
*
* Allows users to construct a Renderbuffer to be attached onto Framebuffers.
*/
class RenderBufferObj {
public:
	
	ObjHandle				Handle;
	RenderBufferType		Type;
	RenderBufferData		*Info;

	RenderBufferObj();
	~RenderBufferObj();

	RenderBufferObj(const RenderBufferObj &Other)				= delete;
	RenderBufferObj& operator= (const RenderBufferObj &Other)	= delete;

	RenderBufferObj(RenderBufferObj &&Other);
	RenderBufferObj& operator= (RenderBufferObj &&Other);

};


/**
* Specify the framebuffer's attachment type.
*/
enum AttachmentType : uint32 {
	FRAMEBUFFER_ATTACHMENT_NONE			= 0x00, // On first init only.
	FRAMEBUFFER_ATTACHMENT_TEXTURE		= 0x01,
	FRAMEBUFFER_ATTACHMENT_RENDERBUFFER = 0x02
};


/**
* Specify the framebuffer's sub attachment type.
*/
enum SubAttachmentType : uint32 {
	FRAMEBUFFER_SUBATTACH_NONE			= 0x00, // On first init only.
	FRAMEBUFFER_SUBATTACH_COLOUR		= 0x01,
	FRAMEBUFFER_SUBATTACH_DEPTH			= 0x02,
	FRAMEBUFFER_SUBATTACH_STENCIL		= 0x03
};


/**
* PostProcessAttachment data structure.
* Specifies the Framebuffer's attachment.
*/
struct PostProcessAttachment {

	union {
		TextureObj			*Texture;
		RenderBufferObj		*Renderbuffer;
	};

	AttachmentType		Type;
	SubAttachmentType	SubType;
	bool				Draw;

	PostProcessAttachment();
	~PostProcessAttachment();

	PostProcessAttachment(const PostProcessAttachment &Other);
	PostProcessAttachment& operator= (const PostProcessAttachment &Other);

	PostProcessAttachment(PostProcessAttachment &&Other);
	PostProcessAttachment& operator= (PostProcessAttachment &&Other);

};


/**
* PostProcessCreationInfo data structure.
* Used to create a Framebuffer for Gravity.
*
* In OpenGL 4.3, it is possible to create a framebuffer with no image attachment.
*/
struct PostProcessCreationInfo {

	Array<PostProcessAttachment> Attachment;
	String		Name;
	int			Width;
	int			Height;

	PostProcessCreationInfo();
	~PostProcessCreationInfo();

	PostProcessCreationInfo(const PostProcessCreationInfo &Other);
	PostProcessCreationInfo& operator= (const PostProcessCreationInfo &Other);

	PostProcessCreationInfo(PostProcessCreationInfo &&Other);
	PostProcessCreationInfo& operator= (PostProcessCreationInfo &&Other);

};


/**
* Gravity's Postprocess data structure.
*
* Allows users to construct a Framebuffer for post-processing effects.
*/
class PostProcessObj {
private:

	using Attachments = Array<PostProcessAttachment>;

public:

	Array<PostProcessAttachment> Attachment;
	PostProcessData *Info;
	ObjHandle		Handle;
	int32			Width;
	int32			Height;

	PostProcessObj();
	~PostProcessObj();

	PostProcessObj(const PostProcessObj &Other)				= delete;
	PostProcessObj& operator= (const PostProcessObj &Other)	= delete;

	PostProcessObj(PostProcessObj &&Other);
	PostProcessObj& operator= (PostProcessObj &&Other);

};