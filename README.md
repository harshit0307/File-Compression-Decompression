
<body>
  <h1>Huffman Compression and Decompression</h1>
  <p>This project implements Huffman Compression and Decompression algorithms in C++.</p>

  <h2>Table of Contents</h2>
  <ul>
      <li><a href="#introduction">Introduction</a></li>
      <li><a href="#features">Features</a></li>
      <li><a href="#requirements">Requirements</a></li>
      <li><a href="#usage">Usage</a></li>
      <li><a href="#example">Example</a></li>
  </ul>

  <h2 id="introduction">Introduction</h2>
  <p>Huffman coding is a lossless data compression algorithm. This project compresses and decompresses files using the Huffman algorithm, which creates a binary tree based on the frequency of each character in the input file.</p>

  <h2 id="features">Features</h2>
  <ul>
      <li>Compresses files using Huffman coding.</li>
      <li>Decompresses files compressed by this program.</li>
      <li>Simple command-line interface for user interaction.</li>
  </ul>

  <h2 id="requirements">Requirements</h2>
  <ul>
      <li>C++ compiler (e.g., g++)</li>
      <li>Standard C++ library</li>
  </ul>

  <h2 id="usage">Usage</h2>
  <p>To use the program, compile the source code and run the executable. The program accepts three commands:</p>
  <ul>
      <li><code>compress &lt;filename&gt;</code>: Compresses the specified file.</li>
      <li><code>decompress &lt;filename&gt;</code>: Decompresses the specified file.</li>
      <li><code>exit</code>: Exits the program.</li>
  </ul>

  <h3>Compiling the Code</h3>
  <pre><code>g++ -o huffman huffman.cpp</code></pre>

  <h3>Running the Program</h3>
  <pre><code>./huffman</code></pre>

  <h2 id="example">Example</h2>
  <p>Here is an example of how to use the program:</p>
  <pre><code>
$ ./huffman
Enter command (compress &lt;filename&gt;, decompress &lt;filename&gt;, exit): compress example.txt
Compression successful: example.mcf
Enter command (compress &lt;filename&gt;, decompress &lt;filename&gt;, exit): decompress example.mcf
Decompression successful: example.txt
Enter command (compress &lt;filename&gt;, decompress &lt;filename&gt;, exit): exit
  </code></pre>
</body>
</html>
