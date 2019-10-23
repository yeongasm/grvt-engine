#pragma once


/**
* Revamp Status: Completed!
*/

/**
*/
enum AttachComponent : uint32 {
	GrvtFramebuffer_AttachComponent_None			= 0xFF, /** On first init only */
	GrvtFramebuffer_AttachComponent_Texture			= 0x00,
	GrvtFramebuffer_AttachComponent_RenderBuffer	= 0x01
};


/**
* Specify the framebuffer's sub attachment type.
*/
enum AttachmentType : uint32 {
	GrvtFramebuffer_Attachment_None			= 0xFF, /** On first init only */
	GrvtFramebuffer_Attachment_Colour		= 0x00,
	GrvtFramebuffer_Attachment_Depth		= 0x01,
	GrvtFramebuffer_Attachment_DepthStencil	= 0x02
};


/**
*/
struct PostProcessAttachment {

	AttachComponent	Component;
	AttachmentType	Type;
	bool			Draw;

	PostProcessAttachment();
	~PostProcessAttachment();

	PostProcessAttachment(AttachComponent, AttachmentType, bool);

};


using PPAttachments = Array<PostProcessAttachment>;


/**
*/
struct PostProcessCreationInfo {

	PPAttachments	Attachments;
	String			Name;
	int32			Width;
	int32			Height;

	/**
	* Pushes a new attachment for the to be created framebuffer.
	*/
	PostProcessAttachment& AddAttachment(AttachComponent, AttachmentType, bool);

	/**
	* Pops a previously added attachment from the to be created framebuffer.
	*/
	void PopAttachment(PostProcessAttachment&);
};


/**
* Revamped GrvtPostProcess object.
*/
class GrvtPostProcess {
private:

	struct AttachmentProperty {

		ObjHandle		Handle;
		AttachComponent Component;
		AttachmentType	Type;
		int32			Count;

	};

public:

	Array<AttachmentProperty> Attachments;
	ObjHandle		Handle;
	int32			Width;
	int32			Height;

	GrvtPostProcess();
	~GrvtPostProcess();

private:

	GrvtPostProcess(const GrvtPostProcess&)				= delete;
	GrvtPostProcess& operator= (const GrvtPostProcess&)	= delete;

	GrvtPostProcess(GrvtPostProcess&&)					= delete;
	GrvtPostProcess& operator= (GrvtPostProcess&&)		= delete;

};