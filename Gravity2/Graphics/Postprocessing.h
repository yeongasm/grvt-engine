#pragma once


class	PostProcess;
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
* [Type] RENDERBUFFER_TYPE_DEPTH_ATTACHMENT = GL_DEPTH_COMPONENT32F.
* [Type] RENDERBUFFER_TYPE_DEPTH_STENCIL_ATTACHMENT = GL_DEPTH24_STENCIL8.
* [Type] RENDERBUFFER_TYPE_COLOUR_ATTACHMENT = GL_RGBA8.
*/
struct RenderBufferCreationInfo {

	String				name;
	RenderBufferType	type;

	RenderBufferCreationInfo();

	RenderBufferCreationInfo(const RenderBufferCreationInfo &Other);
	RenderBufferCreationInfo& operator= (const RenderBufferCreationInfo &Other);

	RenderBufferCreationInfo(RenderBufferCreationInfo &&Other);
	RenderBufferCreationInfo& operator= (RenderBufferCreationInfo &&Other);

	~RenderBufferCreationInfo();

};


/**
* Gravity's OffScreenBuffer data structure.
*
* Allows users to construct a Renderbuffer to be attached onto Framebuffers.
*/
class RenderBuffer {
public:
	
	ObjHandle				handle;
	RenderBufferType		type;
	RenderBufferData		*info;

	RenderBuffer();

	RenderBuffer(const RenderBuffer &Other)				= delete;
	RenderBuffer& operator= (const RenderBuffer &Other)	= delete;

	RenderBuffer(RenderBuffer &&Other);
	RenderBuffer& operator= (RenderBuffer &&Other);

	~RenderBuffer();
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
		Texture			*texture;
		RenderBuffer	*renderbuffer;
	};

	AttachmentType		type;
	SubAttachmentType	subType;
	bool				draw;

	PostProcessAttachment();

	PostProcessAttachment(const PostProcessAttachment &Other);
	PostProcessAttachment& operator= (const PostProcessAttachment &Other);

	PostProcessAttachment(PostProcessAttachment &&Other);
	PostProcessAttachment& operator= (PostProcessAttachment &&Other);

	~PostProcessAttachment();

};


/**
* PostProcessCreationInfo data structure.
* Used to create a Framebuffer for Gravity.
*
* In OpenGL 4.3, it is possible to create a framebuffer with no image attachment.
*/
struct PostProcessCreationInfo {
private:

	using Attachments = Array<PostProcessAttachment>;

public:

	String		name;
	Attachments attachment;
	int			width;
	int			height;

	PostProcessCreationInfo();

	PostProcessCreationInfo(const PostProcessCreationInfo &Other);
	PostProcessCreationInfo& operator= (const PostProcessCreationInfo &Other);

	PostProcessCreationInfo(PostProcessCreationInfo &&Other);
	PostProcessCreationInfo& operator= (PostProcessCreationInfo &&Other);

	~PostProcessCreationInfo();
};


/**
* Gravity's Postprocess data structure.
*
* Allows users to construct a Framebuffer for post-processing effects.
*/
class PostProcess {
private:

	using Attachments = Array<PostProcessAttachment>;

public:

	ObjHandle		id;
	int32			width;
	int32			height;
	PostProcessData *info;
	Attachments		attachment;

	PostProcess();

	PostProcess(const PostProcess &Other)				= delete;
	PostProcess& operator= (const PostProcess &Other)	= delete;

	PostProcess(PostProcess &&Other);
	PostProcess& operator= (PostProcess &&Other);

	~PostProcess();

};