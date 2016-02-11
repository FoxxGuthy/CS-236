----------------------------------------------------------------------------------
-- Engineer: Seth Guthrie

----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

entity vga_timing is
    Port ( clk : in  STD_LOGIC;
           rst : in  STD_LOGIC;
           HS : out  STD_LOGIC;
           VS : out  STD_LOGIC;
           pixel_x : out  STD_LOGIC_VECTOR (9 downto 0);
           pixel_y : out  STD_LOGIC_VECTOR (9 downto 0);
           last_column : out  STD_LOGIC;
           last_row : out  STD_LOGIC;
           blank : out  STD_LOGIC);
end vga_timing;

architecture Behavioral of vga_timing is
	signal pixel_en : std_logic;
	
	signal horizontal_counter : unsigned (9 downto 0):= (others => '0'); -- 0 to 799
	signal vertical_counter : unsigned (9 downto 0) := (others => '0'); -- 0 to 520
	
	signal horizontal_counter_next : unsigned (9 downto 0):= (others => '0'); -- 0 to 799
	signal vertical_counter_next : unsigned (9 downto 0) := (others => '0'); -- 0 to 520
begin

	--reg 1: toggles pixel_en "halving the clock freq"
	process(clk)
	begin
		if (clk'event and clk = '1') then
			if (rst = '1') then
				pixel_en <= '0';
				horizontal_counter <= (others => '0');
				vertical_counter <= (others => '0');
			else
				pixel_en <= not pixel_en; -- toggle pixel_en
					if (pixel_en = '1') then
						horizontal_counter <= horizontal_counter_next;
						vertical_counter <= vertical_counter_next;
					end if; 
			end if; -- rst end if
		end if; -- clk end if
	end process;
	
	--next state logic
	
	horizontal_counter_next <= (others => '0') when horizontal_counter = 799 else
									 horizontal_counter + 1;
										
	vertical_counter_next <= (others => '0') when vertical_counter = 520 and horizontal_counter = 799 else
								 vertical_counter + 1 when horizontal_counter = 799 else
								 vertical_counter;
	
	--last column and HS logic, pixel_x
	last_column <= '1' when horizontal_counter = 639 else -- 1 during column 639
						'0';
						
	HS <= '1' when horizontal_counter < 640 else -- display time
			'1' when horizontal_counter < 656 else -- front porch, 16 pix
			'0' when horizontal_counter < 752 else -- pulse width, 96 pix
			'1'; -- back porch, 48 pix
			
	 pixel_x <= std_logic_vector(horizontal_counter);
			
	-- last_row, VS logic, pixel_y
	last_row <= '1' when vertical_counter = 479 else -- 1 during row 479
					'0';
					
	VS <= '0' when vertical_counter < 492 and vertical_counter > 489 else
			'1';

	 pixel_y <= std_logic_vector(vertical_counter);
	
	--blank signal
	blank <= '1' when (horizontal_counter > 639) or (vertical_counter > 479) else
				'0'; -- when on screen

end Behavioral;

