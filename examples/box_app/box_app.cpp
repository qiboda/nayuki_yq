// // ***************************************************************************************
// // BoxApp.cpp by Frank Luna (C) 2015 All Rights Reserved.
// //
// // Shows how to draw a box in Direct3D 12.
// //
// // Controls:
// // Hold the left mouse button down and move the mouse to rotate.
// // Hold the right mouse button down and move the mouse to zoom in and out.
// // ***************************************************************************************
// #include "NayukiYq/NayukiYq.h"

// #include "Core/MathHelper.h"
// #include "Core/UploadBuffer.h"
// #include "Core/D3dApp.h"

// using Microsoft::WRL::ComPtr;
// using namespace DirectX;
// using namespace PackedVector;

// struct Vertex
// {
//     glm::vec3 Pos;
//     XMFLOAT4 Color;
// };

// struct VertexPos
// {
//     glm::vec3 Pos;
// };

// struct VertexColor
// {
//     XMFLOAT4 Color;
// };

// struct ObjectConstants
// {
//     XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
// };

// class BoxApp
//     : public D3DApp
// {
// public:
//     BoxApp( HINSTANCE hInstance );
//     BoxApp( const BoxApp& rhs ) = delete;
//     BoxApp& operator = ( const BoxApp& rhs ) = delete;
//     ~BoxApp();

//     virtual bool Initialize() override;

// private:
//     virtual void OnResize() override;
//     virtual void Update( const GameTimer& gt ) override;
//     virtual void Draw( const GameTimer& gt ) override;

//     virtual void OnMouseDown( WPARAM btnState, i32 x, i32 y ) override;
//     virtual void OnMouseUp( WPARAM btnState, i32 x, i32 y ) override;
//     virtual void OnMouseMove( WPARAM btnState, i32 x, i32 y ) override;

//     void BuildDescriptorHeaps();
//     void BuildConstantBuffers();
//     void BuildRootSignature();
//     void BuildShadersAndInputLayout();
//     void BuildBoxGeometry(bool splitVertex);
//     void BuildPSO();

// private:
//     ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
//     ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

//     std::unique_ptr<UploadBuffer<ObjectConstants> > mObjectCB = nullptr;

//     std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

//     ComPtr<ID3DBlob> mvsByteCode = nullptr;
//     ComPtr<ID3DBlob> mpsByteCode = nullptr;

//     std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

//     // pipeline state: vs, ps and other state and some settings.
//     ComPtr<ID3D12PipelineState> mPSO = nullptr;

//     XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
//     XMFLOAT4X4 mView = MathHelper::Identity4x4();
//     XMFLOAT4X4 mProj = MathHelper::Identity4x4();

//     f32 mTheta = 1.5f * XM_PI;
//     f32 mPhi = XM_PIDIV4;
//     f32 mRadius = 5.0f;

//     POINT mLastMousePos {};
// };

// int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
//                     _In_ int nShowCmd )
// {
//     BoxApp theApp( hInstance );
//     if ( !theApp.Initialize() )
//     {
//         return 0;
//     }

//     return theApp.Run();
// }

// BoxApp::BoxApp( HINSTANCE hInstance )
//     : D3DApp( hInstance ) {}

// BoxApp::~BoxApp() {}

// bool BoxApp::Initialize()
// {
//     if ( !D3DApp::Initialize() )
//     {
//         return false;
//     }

//     // Reset the command list to prep for initialization commands.
//     NY_ASSERT_MSG_HR( mCommandList->Reset( mDirectCmdListAlloc.Get(), nullptr ) );

//     BuildDescriptorHeaps();
//     BuildConstantBuffers();
//     BuildRootSignature();
//     BuildShadersAndInputLayout();
//     BuildBoxGeometry(false);
//     BuildPSO();

//     // Execute the initialization commands.
//     // Must close before execute commands.
//     NY_ASSERT_MSG_HR( mCommandList->Close() );
//     std::array<ID3D12CommandList*, 1> cmdsLists = { mCommandList.Get() };
//     mCommandQueue->ExecuteCommandLists( static_cast<u32>( cmdsLists.size() ), cmdsLists.data() );

//     // Wait until initialization is complete.
//     FlushCommandQueue();

//     return true;
// }

// void BoxApp::OnResize()
// {
//     D3DApp::OnResize();

//     // The window resized, so update the aspect ratio and recompute the projection matrix.
//     XMMATRIX P = XMMatrixPerspectiveFovLH( 0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f );
//     XMStoreFloat4x4( &mProj, P );
// }

// void BoxApp::Update( const GameTimer& gt )
// {
//     // Convert Spherical to Cartesian coordinates.
//     f32 x = mRadius * sinf( mPhi ) * cosf( mTheta );
//     f32 z = mRadius * sinf( mPhi ) * sinf( mTheta );
//     f32 y = mRadius * cosf( mPhi );

//     // Build the view matrix.
//     glm::vec3 pos = glm::vec3Set( x, y, z, 1.0f );
//     glm::vec3 target = glm::vec3Zero();
//     glm::vec3 up = glm::vec3Set( 0.0f, 1.0f, 0.0f, 0.0f );

//     XMMATRIX view = XMMatrixLookAtLH( pos, target, up );
//     XMStoreFloat4x4( &mView, view );

//     XMMATRIX world = XMLoadFloat4x4( &mWorld );
//     XMMATRIX proj = XMLoadFloat4x4( &mProj );
//     XMMATRIX worldViewProj = world * view * proj;

//     // Update the constant buffer with the latest worldViewProj matrix.
//     ObjectConstants objConstants;
//     XMStoreFloat4x4( &objConstants.WorldViewProj, XMMatrixTranspose( worldViewProj ) );
//     mObjectCB->CopyData( 0, objConstants );
// }

// void BoxApp::Draw( const GameTimer& gt )
// {
//     // Reuse the memory associated with command recording.
//     // We can only reset when the associated command lists have finished execution on the GPU.
//     NY_ASSERT_MSG_HR( mDirectCmdListAlloc->Reset() );

//     // A command list can be reset after it has been added to the command queue via
//     // ExecuteCommandList.
//     // Reusing the command list reuses memory.
//     NY_ASSERT_MSG_HR( mCommandList->Reset( mDirectCmdListAlloc.Get(), mPSO.Get() ) );

//     // RS: The Rasterizer Stage
//     mCommandList->RSSetViewports( 1, &mScreenViewport );
//     mCommandList->RSSetScissorRects( 1, &mScissorRect );

//     // Indicate a state transition on the resource usage.
//     D3D12_RESOURCE_BARRIER ResourceBarrier_Present2RenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
//         CurrentBackBuffer(),
//         D3D12_RESOURCE_STATE_PRESENT,
//         D3D12_RESOURCE_STATE_RENDER_TARGET
//     );
//     mCommandList->ResourceBarrier( 1, &ResourceBarrier_Present2RenderTarget );

//     D3D12_CPU_DESCRIPTOR_HANDLE BackBufferDescriptorHandle = CurrentBackBufferView();
//     D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilDescriptorHandle = DepthStencilView();

//     // Clear the back buffer and depth buffer. Call Every Draw.
//     // The nullptr means clear the entire render target, or clear the specified rectanglar areas.
//     mCommandList->ClearRenderTargetView( BackBufferDescriptorHandle, Colors::LightSteelBlue, 0, nullptr );
//     mCommandList->ClearDepthStencilView(
//         DepthStencilDescriptorHandle,
//         D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
//         1.0f,
//         0,
//         0,
//         nullptr
//     );

//     // Specify the buffers we are going to render to.
//     // OM: The Output Merger stage.
//     mCommandList->OMSetRenderTargets( 1, &BackBufferDescriptorHandle, true, &DepthStencilDescriptorHandle );

//     std::array<ID3D12DescriptorHeap*, 1> descriptorHeaps = { mCbvHeap.Get() };
//     mCommandList->SetDescriptorHeaps( descriptorHeaps.size(), descriptorHeaps.data() );

//     mCommandList->SetGraphicsRootSignature( mRootSignature.Get() );

//     D3D12_VERTEX_BUFFER_VIEW BoxGeoVertexBufferView = mBoxGeo->VertexBufferView();
//     D3D12_INDEX_BUFFER_VIEW BoxGeoIndexBufferView = mBoxGeo->IndexBufferView();
//     mCommandList->IASetVertexBuffers( 0, 1, &BoxGeoVertexBufferView );
//     mCommandList->IASetIndexBuffer( &BoxGeoIndexBufferView );
//     mCommandList->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

//     mCommandList->SetGraphicsRootDescriptorTable( 0, mCbvHeap->GetGPUDescriptorHandleForHeapStart() );

//     mCommandList->DrawIndexedInstanced( mBoxGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0 );

//     // Indicate a state transition on the resource usage.
//     D3D12_RESOURCE_BARRIER ResourceBarrier_RenderTarget2Present = CD3DX12_RESOURCE_BARRIER::Transition(
//         CurrentBackBuffer(),
//         D3D12_RESOURCE_STATE_RENDER_TARGET,
//         D3D12_RESOURCE_STATE_PRESENT
//     );
//     mCommandList->ResourceBarrier( 1, &ResourceBarrier_RenderTarget2Present );

//     // Done recording commands.
//     NY_ASSERT_MSG_HR( mCommandList->Close() );

//     // Add the command list to the queue for execution.
//     std::array<ID3D12CommandList*, 1> cmdsLists { mCommandList.Get() };
//     mCommandQueue->ExecuteCommandLists( static_cast<u32>( cmdsLists.size() ), cmdsLists.data() );

//     // swap the back and front buffers
//     NY_ASSERT_MSG_HR( mSwapChain->Present( 0, 0 ) );
//     mCurrBackBuffer = ( mCurrBackBuffer + 1 ) % SwapChainBufferCount;

//     // Wait until frame commands are complete.  This waiting is inefficient and is
//     // done for simplicity.  Later we will show how to organize our rendering code
//     // so we do not have to wait per frame.
//     FlushCommandQueue();
// }

// void BoxApp::OnMouseDown( WPARAM btnState, i32 x, i32 y )
// {
//     mLastMousePos.x = x;
//     mLastMousePos.y = y;

//     SetCapture( mhMainWnd );
// }

// void BoxApp::OnMouseUp( WPARAM btnState, i32 x, i32 y ) { ReleaseCapture(); }

// void BoxApp::OnMouseMove( WPARAM btnState, i32 x, i32 y )
// {
//     if ( ( btnState & MK_LBUTTON ) != 0 )
//     {
//         // Make each pixel correspond to a quarter of a degree.
//         f32 dx = XMConvertToRadians( 0.25f * static_cast<f32>( x - mLastMousePos.x ) );
//         f32 dy = XMConvertToRadians( 0.25f * static_cast<f32>( y - mLastMousePos.y ) );

//         // Update angles based on input to orbit camera around box.
//         mTheta += dx;
//         mPhi += dy;

//         // Restrict the angle mPhi.
//         mPhi = MathHelper::Clamp( mPhi, 0.1f, MathHelper::Pi - 0.1f );
//     }
//     else if ( ( btnState & MK_RBUTTON ) != 0 )
//     {
//         // Make each pixel correspond to 0.005 unit in the scene.
//         f32 dx = 0.005f * static_cast<f32>( x - mLastMousePos.x );
//         f32 dy = 0.005f * static_cast<f32>( y - mLastMousePos.y );

//         // Update the camera radius based on input.
//         mRadius += dx - dy;

//         // Restrict the radius.
//         mRadius = MathHelper::Clamp( mRadius, 3.0f, 15.0f );
//     }

//     mLastMousePos.x = x;
//     mLastMousePos.y = y;
// }

// void BoxApp::BuildDescriptorHeaps()
// {
//     D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
//     cbvHeapDesc.NumDescriptors = 1;
//     cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//     cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  // Can visible in shader file.
//     cbvHeapDesc.NodeMask = 0;
//     NY_ASSERT_MSG_HR( md3dDevice->CreateDescriptorHeap( &cbvHeapDesc, IID_PPV_ARGS( &mCbvHeap ) ) );
// }

// void BoxApp::BuildConstantBuffers()
// {
//     // Pass data(mOjbectCB) to shader.
//     mObjectCB = std::make_unique<UploadBuffer<ObjectConstants> >( md3dDevice.Get(), 1, true );

//     D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
//     cbvDesc.BufferLocation = mObjectCB->Resource()->GetGPUVirtualAddress();
//     cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize( sizeof( ObjectConstants ) );
//     md3dDevice->CreateConstantBufferView( &cbvDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart() );
// }

// void BoxApp::BuildRootSignature()
// {
//     // Shader programs typically require resources as input (constant buffers,
//     // textures, samplers).  The root signature defines the resources the shader
//     // programs expect.  If we think of the shader programs as a function, and
//     // the input resources as function parameters, then the root signature can be
//     // thought of as defining the function signature.

//     // Root parameter can be a table, root descriptor or root constants.
//     CD3DX12_ROOT_PARAMETER slotRootParameter[1] = {};

//     // Create a single descriptor table of CBVs.
//     // Shader parameter is register(b0).
//     CD3DX12_DESCRIPTOR_RANGE cbvRange;
//     cbvRange.Init( D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0 );
//     slotRootParameter[0].InitAsDescriptorTable( 1, &cbvRange );

//     // A root signature is an array of root parameters.
//     CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc( 1, slotRootParameter, 0, nullptr,
//                                              D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT );

//     // create a root signature with a single slot which points to a descriptor range consisting of a
//     // single constant buffer
//     ComPtr<ID3DBlob> serializedRootSig = nullptr;
//     ComPtr<ID3DBlob> errorBlob = nullptr;
//     HRESULT hr = D3D12SerializeRootSignature(
//         &rootSigDesc,
//         D3D_ROOT_SIGNATURE_VERSION_1,
//         serializedRootSig.GetAddressOf(),
//         errorBlob.GetAddressOf()
//     );

//     if ( errorBlob != nullptr )
//     {
//         ::OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
//     }
//     NY_ASSERT_MSG_HR( hr );

//     NY_ASSERT_MSG_HR(
//         md3dDevice->CreateRootSignature(
//             0,
//             serializedRootSig->GetBufferPointer(),
//             serializedRootSig->GetBufferSize(),
//             IID_PPV_ARGS( &mRootSignature )
//         )
//     );
// }

// void BoxApp::BuildShadersAndInputLayout()
// {
//     mvsByteCode = d3dUtil::CompileShader( L"Shaders\\color.hlsl", nullptr, "VS", "vs_5_0" );
//     mpsByteCode = d3dUtil::CompileShader( L"Shaders\\color.hlsl", nullptr, "PS", "ps_5_0" );

//     mInputLayout =
//     {
//         {
//             "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//         },
//         {
//             "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//         }
//     };
// }

// void BoxApp::BuildBoxGeometry(bool splitVertex)
// {
//     std::array<Vertex, 8> vertices;
//     std::array<VertexPos, 8> verticesPoses;
//     std::array<VertexColor, 8> verticesColors;

//     if (splitVertex)
//     {
//         // DISABLE-FORMAT
//         verticesPoses[0].Pos = glm::vec3(-1.0f, -1.0f, -1.0f);
//         verticesPoses[1].Pos = glm::vec3(-1.0f, +1.0f, -1.0f);
//         verticesPoses[2].Pos = glm::vec3(+1.0f, +1.0f, -1.0f);
//         verticesPoses[3].Pos = glm::vec3(+1.0f, -1.0f, -1.0f);
//         verticesPoses[4].Pos = glm::vec3(-1.0f, -1.0f, +1.0f);
//         verticesPoses[5].Pos = glm::vec3(-1.0f, +1.0f, +1.0f);
//         verticesPoses[6].Pos = glm::vec3(+1.0f, +1.0f, +1.0f);
//         verticesPoses[7].Pos = glm::vec3(+1.0f, -1.0f, +1.0f);
//         // ENABLE-FORMAT

//             // DISABLE-FORMAT
//         verticesColors[0].Color = XMFLOAT4(Colors::White);
//         verticesColors[1].Color = XMFLOAT4(Colors::Black);
//         verticesColors[2].Color = XMFLOAT4(Colors::Red);
//         verticesColors[3].Color = XMFLOAT4(Colors::Green);
//         verticesColors[4].Color = XMFLOAT4(Colors::Blue);
//         verticesColors[5].Color = XMFLOAT4(Colors::Yellow);
//         verticesColors[6].Color = XMFLOAT4(Colors::Cyan);
//         verticesColors[7].Color = XMFLOAT4(Colors::Magenta);
//         // ENABLE-FORMAT
//     }
//     else
//     {
//         // DISABLE-FORMAT
//         vertices[0] = Vertex({ glm::vec3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) });
//         vertices[1] = Vertex({ glm::vec3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) });
//         vertices[2] = Vertex({ glm::vec3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) });
//         vertices[3] = Vertex({ glm::vec3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) });
//         vertices[4] = Vertex({ glm::vec3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) });
//         vertices[5] = Vertex({ glm::vec3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) });
//         vertices[6] = Vertex({ glm::vec3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) });
//         vertices[7] = Vertex({ glm::vec3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) });
//         // ENABLE-FORMAT
//     }

//     std::array<ui16, 36> indices =
//     {
//         // front face
//         0u, 1u, 2u, 0u, 2u, 3u,
//         // back face
//         4u, 6u, 5u, 4u, 7u, 6u,
//         // left face
//         4u, 5u, 1u, 4u, 1u, 0u,
//         // right face
//         3u, 2u, 6u, 3u, 6u, 7u,
//         // top face
//         1u, 5u, 6u, 1u, 6u, 2u,
//         // bottom face
//         4u, 0u, 3u, 4u, 3u, 7u,
//     };

//     const u32 vbByteSize = (u32)vertices.size() * sizeof( Vertex );
//     const u32 vbpByteSize = (u32)verticesPoses.size() * sizeof( VertexPos );
//     const u32 vbcByteSize = (u32)verticesColors.size() * sizeof( VertexColor );
//     const u32 ibByteSize = (u32)indices.size() * sizeof( ui16 );

//     if (mBoxGeo == nullptr)
//     {
//         mBoxGeo = std::make_unique<MeshGeometry>();
//     }
//     mBoxGeo->Name = "boxGeo";

//     if (splitVertex)
//     {
//         NY_ASSERT_MSG_HR(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
//         CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
//     }
//     else
//     {
//         NY_ASSERT_MSG_HR(D3DCreateBlob(vbpByteSize + vbcByteSize, &mBoxGeo->VertexBufferCPU));
//         CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), verticesPoses.data(), vbpByteSize);
//         CopyMemory((char*)mBoxGeo->VertexBufferCPU->GetBufferPointer() + vbpByteSize, verticesColors.data(),
//         vbcByteSize);
//     }

//     NY_ASSERT_MSG_HR( D3DCreateBlob( ibByteSize, &mBoxGeo->IndexBufferCPU ) );
//     CopyMemory( mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize );

//     mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(
//         md3dDevice.Get(),
//         mCommandList.Get(),
//         vertices.data(),
//         vbByteSize,
//         mBoxGeo->VertexBufferUploader
//     );

//     mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(
//         md3dDevice.Get(),
//         mCommandList.Get(),
//         indices.data(),
//         ibByteSize,
//         mBoxGeo->IndexBufferUploader
//     );

//     mBoxGeo->VertexByteStride = sizeof( Vertex );
//     mBoxGeo->VertexBufferByteSize = vbByteSize;
//     mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
//     mBoxGeo->IndexBufferByteSize = ibByteSize;

//     SubmeshGeometry submesh;
//     submesh.IndexCount = static_cast<u32>( indices.size() );
//     submesh.StartIndexLocation = 0;
//     submesh.BaseVertexLocation = 0;

//     mBoxGeo->DrawArgs["box"] = submesh;
// }

// void BoxApp::BuildPSO()
// {
//     D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
//     ZeroMemory( &psoDesc, sizeof( D3D12_GRAPHICS_PIPELINE_STATE_DESC ) );
//     psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
//     psoDesc.pRootSignature = mRootSignature.Get();  // TODO: what is this?
//     psoDesc.VS =
//     {
//         reinterpret_cast<u8*>( mvsByteCode->GetBufferPointer() ), mvsByteCode->GetBufferSize()
//     };
//     psoDesc.PS =
//     {
//         reinterpret_cast<u8*>( mpsByteCode->GetBufferPointer() ), mpsByteCode->GetBufferSize()
//     };
//     psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC( D3D12_DEFAULT );
//     psoDesc.BlendState = CD3DX12_BLEND_DESC( D3D12_DEFAULT );
//     psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC( D3D12_DEFAULT );
//     psoDesc.SampleMask = std::numeric_limits<u32>::max();
//     psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//     psoDesc.NumRenderTargets = 1;
//     psoDesc.RTVFormats[0] = mBackBufferFormat;
//     psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
//     psoDesc.SampleDesc.Quality = m4xMsaaState ? ( m4xMsaaQuality - 1 ) : 0;
//     psoDesc.DSVFormat = mDepthStencilFormat;
//     NY_ASSERT_MSG_HR( md3dDevice->CreateGraphicsPipelineState( &psoDesc, IID_PPV_ARGS( &mPSO ) ) );
// }

void main()
{
    return;
}