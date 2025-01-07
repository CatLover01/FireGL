#include <FireGL/Renderer/Renderer.h>
#include <FireGL/Renderer/SceneObject.h>
#include <FireGL/Renderer/Scene.h>

#include <External/glad/glad.h>

namespace fgl
{

	Renderer::Renderer(RenderingMode Mode)
	{
		ConfigureRenderingMode(Mode);
		SetupBuffer();
	}

	Renderer::~Renderer()
	{
		CleanupBuffer();
	}

	void Renderer::ConfigureRenderingMode(RenderingMode NewMode)
	{
		switch (NewMode)
		{
		case RenderingMode::Default:
			glEnable(GL_DEPTH_TEST);
			break;
		case RenderingMode::DebugLine:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case RenderingMode::DebugFill:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
		glEnable(GL_MULTISAMPLE);
	}

	void Renderer::SetupBuffer()
	{
		glGenBuffers(1, &m_MVPBufferID);
	}

	void Renderer::CleanupBuffer()
	{
		glDeleteBuffers(1, &m_MVPBufferID);
	}

	void Renderer::Render(Scene* Scene)
	{
		ClearFrameBuffer();
		SceneObject* Skybox = nullptr;
		auto ObjectBatches = BatchSceneObjects(Scene, Skybox);

		UpdateMVPInstances(Scene, ObjectBatches, Skybox != nullptr);
		RenderBatches(ObjectBatches);
		RenderSkybox(Skybox);
	}

	void Renderer::ClearFrameBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	std::map<size_t, std::vector<SceneObject*>> Renderer::BatchSceneObjects(Scene* Scene, SceneObject*& Skybox)
	{
		std::map<size_t, std::vector<SceneObject*>> ObjectBatches;
		for (const auto& Object : Scene->GetObjects())
		{
			if (Object->IsSkybox())
			{
				Skybox = Object.get();
				continue;
			}
			size_t VertexHash = Object->GetHash();
			ObjectBatches[VertexHash].push_back(Object.get());
		}
		return ObjectBatches;
	}

	void Renderer::RenderBatches(const std::map<size_t, std::vector<SceneObject*>>& ObjectBatches)
	{
		for (const auto& [Hash, ObjectBatch] : ObjectBatches)
		{
			for (SceneObject* Object : ObjectBatch)
			{
				Object->Render(ObjectBatch.size());
			}
		}
	}

	void Renderer::RenderSkybox(SceneObject* Skybox)
	{
		if (!Skybox)
			return;

		if (Skybox->IsNew())
		{
			PerformFirstPass(Skybox);
			Skybox->SetNew(false);
		}
		Skybox->Render(1);
	}

	void Renderer::PerformFirstPass(SceneObject* Object)
	{
		for (BaseMesh& Mesh : Object->GetMeshes())
		{
			Mesh.FirstPass();
		}
	}

	void Renderer::PerformSecondPass(SceneObject* Object)
	{
		for (BaseMesh& Mesh : Object->GetMeshes())
		{
			Mesh.SecondPass();
		}
	}

	void Renderer::UpdateMVPInstances(const Scene* Scene, const std::map<size_t, std::vector<SceneObject*>>& ObjectBatches, bool HasSkybox)
	{
		size_t TotalObjectCount = Scene->GetObjects().size() - HasSkybox;

		EnsureBufferCapacity(TotalObjectCount);

		size_t Index = 0;
		for (const auto& [Hash, Batch] : ObjectBatches)
		{
			for (SceneObject* Object : Batch)
			{
				ProcessObjectForMVP(Object, Index);
			}
		}

		UploadMVPDataToGPU();
	}

	void Renderer::EnsureBufferCapacity(size_t TotalObjectCount)
	{
		if (!m_MVPMatrixBuffer.Get() || m_MVPMatrixBuffer.GetObjectCount() < TotalObjectCount)
		{
			m_MVPMatrixBuffer.Resize(TotalObjectCount);
			glBindBuffer(GL_ARRAY_BUFFER, m_MVPBufferID);
			glBufferData(GL_ARRAY_BUFFER, m_MVPMatrixBuffer.GetBufferSize(), nullptr, GL_DYNAMIC_DRAW);
		}
	}

	void Renderer::ProcessObjectForMVP(SceneObject* Object, size_t& Index)
	{
		if (Object->IsNew())
		{
			PerformFirstPass(Object);
			BindMVPBuffer();
			PerformSecondPass(Object);
			Object->SetNew(false);
		}

		glm::mat4 MVP, Model;
		Object->GetTransform().ComputeModelViewProjection(MVP, Model);
		m_MVPMatrixBuffer.Get()[Index] = MVP;
		m_MVPMatrixBuffer.Get()[Index + 1] = Model;
		Index += 2;
	}

	void Renderer::BindMVPBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_MVPBufferID);
	}

	void Renderer::UploadMVPDataToGPU()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_MVPBufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_MVPMatrixBuffer.GetBufferSize(), m_MVPMatrixBuffer.Get());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} // namespace fgl