Code for "How to Build a BVH"<br><br>

<b>INSTRUCTIONS:</b> Open the "_ bvhdemo.sln" file (all the way at the top!), and make one of the 10 projects the active project. 

<b>Note:</b> project files are for Visual Studio 2019; they will convert without issues to 2022 though.

Each project implements one of the 10 articles:

<b>part 1, basics:</b><br>
https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics<br>
<i>...Which explains how to set up a minimal but working BVH in about 300 lines of code.</i><br>
Project: basics.vcxproj, files: basics.cpp, basics.h<br><br>

<b>part 2, faster rays:</b><br>
https://jacco.ompf2.com/2022/04/18/how-to-build-a-bvh-part-2-faster-rays<br>
<i>...in which rays are traced faster, at the expense of BVH build time.</i><br>
Project: faster.vcxproj, files: faster.cpp, faster.h<br><br>

<b>part 3, quick builds:</b><br>
https://jacco.ompf2.com/2022/04/21/how-to-build-a-bvh-part-3-quick-builds<br>
<i>...explains how a high-quality BVH can be constructed rapidly.</i><br>
Project: quickbuild.vcxproj, files: quickbuild.cpp, quickbuild.h<br><br>

<b>part 4, animation:</b><br>
https://jacco.ompf2.com/2022/04/26/how-to-build-a-bvh-part-4-animation<br>
<i>...in which various forms of animation are applied to BVHs.</i><br>
Project: animation.vcxproj, files: animation.cpp, animation.h<br><br>

<b>part 5, TLAS & BLAS:</b><br>
https://jacco.ompf2.com/2022/05/07/how-to-build-a-bvh-part-5-tlas-blas<br>
<i>...describes how a lot of BVHs can be made into one.</i><br>
Project: toplevel.vcxproj, files: toplevel.cpp, toplevel.h<br><br>

<b>part 6, TLAS & BLAS part 2:</b><br>
https://jacco.ompf2.com/2022/05/13/how-to-build-a-bvh-part-6-all-together-now<br>
<i>...which completes the discussion of article 5.</i><br>
Project: alltogether.vcxproj, files: alltogether.cpp, alltogether.h<br><br>

<b>part 7, consolidation:</b><br>
https://jacco.ompf2.com/2022/05/20/how-to-build-a-bvh-part-7-consolidate<br>
<i>...in which the BVH is applied to some nicer renders.</i><br>
Project: pretty.vcxproj, files: pretty.cpp, pretty.h, bvh.cpp, bvh.h<br><br>

<b>part 8, whitted:</b><br>
https://jacco.ompf2.com/2022/05/27/how-to-build-a-bvh-part-8-whitted-style<br>
<i>...completing part 7, with full recursive ray tracing, in real-time.</i><br>
Project: whitted.vcxproj, files: whitted.cpp, whitted.h, bvh.*<br><br>

<b>part 9a, GPGPU:</b><br>
https://jacco.ompf2.com/2022/06/03/how-to-build-a-bvh-part-9a-to-the-gpu<br>
<i>...which is really a GPGPU tutorial, using BVHs as an example.</i><br>
Project: gpgpu.vcxproj, files: gpgpu.cpp, gpgpu.h, bvh.*<br><br>

<b>part 9b, 'massive':</b><br>
https://jacco.ompf2.com/2022/06/15/how-to-build-a-bvh-part-9b-massive<br>
<i>...series finale, with TLAS & BLAS on the GPU. Also: GL/CL interop.</i><br>
Project: massive.vcxproj, files: massive.cpp, massive.h, raytracer.cl.<br><br>

NOTE: All projects share the same template files and build directories.<br>
DISCLAIMER: None of this is supposed to be 'production quality'.<br>
LICENSE: This code is covered by the Unlicense. Feel free, no strings.<br><br>
P.S.: Follow me on Twitter, <a href="https://twitter.com/j_bikker">@j_bikker</a>.
