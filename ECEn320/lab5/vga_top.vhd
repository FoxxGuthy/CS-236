library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity vga_top is
    Port ( clk : in  STD_LOGIC;
           btn : in  STD_LOGIC_VECTOR (3 downto 0);
           sw : in  STD_LOGIC_VECTOR (7 downto 0);
           Hsync : out  STD_LOGIC;
           Vsync : out  STD_LOGIC;
           vgaRed : out  STD_LOGIC_VECTOR (2 downto 0);
           vgaGreen : out  STD_LOGIC_VECTOR (2 downto 0);
           vgaBlue : out  STD_LOGIC_VECTOR (1 downto 0)
			  );
end vga_top;

architecture Behavioral of vga_top is

	component vga_timing
		port(
		clk : in  STD_LOGIC;
		  rst : in  STD_LOGIC;
		  HS : out  STD_LOGIC;
		  VS : out  STD_LOGIC;
		  pixel_x : out  STD_LOGIC_VECTOR (9 downto 0);
		  pixel_y : out  STD_LOGIC_VECTOR (9 downto 0);
		  last_column : out  STD_LOGIC;
		  last_row : out  STD_LOGIC;
		  blank : out  STD_LOGIC
		  );
	end component;
		
	signal top_pixel_x, top_pixel_y : std_logic_vector (9 downto 0);
	signal top_last_row, top_last_column, top_blank, Hnext, Vnext : std_logic;
	signal red_next, red, green_next, green : std_logic_vector (2 downto 0);
	signal blue_next, blue : std_logic_vector (1 downto 0);
	
begin
	main : vga_timing
	port map(
	clk, btn(3), Hnext , Vnext, top_pixel_x, top_pixel_y,
	top_last_column , top_last_row , top_blank 
	);

	--registers
	process(clk)
	begin
		if (clk'event and clk = '1') then
			vgaRed <= red_next;
			vgaGreen <= green_next;
			vgaBlue <= blue_next;
			Hsync <= Hnext;
			Vsync <= Vnext;
		end if;
	end process;
	
	--pixel color logic
	red <= "111" when btn(2) = '1' else
					"000" when btn(1) = '1' else
					"000" when btn(0) = '1' else
					sw(7 downto 5);
					
	green <= "000" when btn(2) = '1' else
					"111" when btn(1) = '1' else
					"000" when btn(0) = '1' else
					sw(4 downto 2);
					
	blue <= "00" when btn(2) = '1' else
					"00" when btn(1) = '1' else
					"11" when btn(0) = '1' else
					sw(1 downto 0);
	
	--blank
	red_next <= red when top_blank = '0' else "000";
	green_next <= green when top_blank = '0' else "000";
	blue_next <= blue when top_blank = '0' else "00";

end Behavioral;

